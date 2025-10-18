from flask import Flask, request, jsonify
import quant_risk_engine

app = Flask(__name__)

@app.route('/calculate_risk', methods=['POST'])
def calculate_risk():
    data = request.get_json()
    
    if not data or 'portfolio' not in data or 'market_data' not in data:
        return jsonify({'error': 'Invalid input format'}), 400

    try:
        portfolio_data = data['portfolio']
        market_data_map_py = data['market_data']

        portfolio = quant_risk_engine.Portfolio()
        for item in portfolio_data:
            option_type = quant_risk_engine.OptionType.Call if item['type'].lower() == 'call' else quant_risk_engine.OptionType.Put
            option = quant_risk_engine.EuropeanOption(
                option_type,
                item['strike'],
                item['expiry'],
                item['asset_id']
            )
            portfolio.add_instrument(option, item['quantity'])

        market_data_map_cpp = {}
        for asset_id, md_py in market_data_map_py.items():
            md_cpp = quant_risk_engine.MarketData()
            md_cpp.asset_id = asset_id
            md_cpp.spot_price = md_py['spot']
            md_cpp.risk_free_rate = md_py['rate']
            md_cpp.volatility = md_py['vol']
            market_data_map_cpp[asset_id] = md_cpp

        engine = quant_risk_engine.RiskEngine()
        result_cpp = engine.calculate_portfolio_risk(portfolio, market_data_map_cpp)

        result_py = {
            'total_pv': result_cpp.total_pv,
            'total_delta': result_cpp.total_delta,
            'total_gamma': result_cpp.total_gamma,
            'total_vega': result_cpp.total_vega,
            'total_theta': result_cpp.total_theta,
            'value_at_risk_95': result_cpp.value_at_risk_95,
        }
        return jsonify(result_py)

    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True, port=5000)
