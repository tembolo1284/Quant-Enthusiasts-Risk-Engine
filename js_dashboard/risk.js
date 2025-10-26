/**
 * ========================================
 * RISK.JS - Risk Calculation
 * ========================================
 */

const calculateRisk = async () => {
    if (STATE.calculating) return;
    if (STATE.portfolio.length === 0) {
        showError('Portfolio is empty. Add instruments before calculating risk.');
        return;
    }

    STATE.calculating = true;
    ELEMENTS.calculateBtn.disabled = true;
    showSpinner();

    // Send empty market_data object - API will auto-fetch
    const payload = {
        portfolio: STATE.portfolio,
        market_data: {},  // Empty - let API auto-fetch
        var_parameters: {
            simulations: 100000,
            confidence: 0.95,
            time_horizon: 1.0
        }
    };

    try {
        const response = await fetch(`${CONFIG.API_BASE_URL}/calculate_risk`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        });

        if (!response.ok) {
            const errorData = await response.json();
            throw new Error(errorData.error || `HTTP ${response.status}: Calculation failed`);
        }

        const results = await response.json();

        // Update cache with auto-fetched data
        if (results.market_data_info && results.market_data_info.market_data_used) {
            for (const [ticker, data] of Object.entries(results.market_data_info.market_data_used)) {
                STATE.marketDataCache[ticker] = {
                    ...data,
                    last_updated: new Date().toISOString()
                };
            }
            updateMarketDataDisplay();
        }

        // Show auto-fetched assets info
        if (results.market_data_info && results.market_data_info.auto_fetched_assets.length > 0) {
            ELEMENTS.autoFetchedAssets.textContent = results.market_data_info.auto_fetched_assets.join(', ');
            ELEMENTS.autoFetchInfo.classList.remove('hidden');
        } else {
            ELEMENTS.autoFetchInfo.classList.add('hidden');
        }

        const metrics = [
            { label: 'Total PV', value: results.total_pv, prefix: '$' },
            { label: 'Delta', value: results.total_delta },
            { label: 'Gamma', value: results.total_gamma },
            { label: 'Vega', value: results.total_vega },
            { label: 'Theta', value: results.total_theta },
            { label: '95% VaR (1d)', value: results.value_at_risk_95, prefix: '$' }
        ];

        ELEMENTS.resultsContainer.innerHTML = metrics.map(({ label, value, prefix = '' }) => `
            <div class="metric-card bg-gray-800/50 p-4 md:p-5 rounded-xl border border-gray-700/60 hover:border-cyan-500/50 transition">
                <p class="text-xs text-gray-400 mb-2 uppercase tracking-wide">${label}</p>
                <p class="text-xl md:text-2xl font-bold ${getMetricColor(label, value)}">${prefix}${formatValue(value)}</p>
            </div>
        `).join('');

        ELEMENTS.resultsContainer.classList.remove('hidden');
        ELEMENTS.initialState.classList.add('hidden');
        ELEMENTS.errorMessage.classList.add('hidden');

        // Update health check to show updated cache
        await checkApiHealth();

    } catch (error) {
        showError(`Error: ${error.message}`);
    } finally {
        hideSpinner();
        STATE.calculating = false;
        ELEMENTS.calculateBtn.disabled = !STATE.apiOnline || STATE.portfolio.length === 0;
    }
};

const setupRiskCalculation = () => {
    ELEMENTS.calculateBtn.addEventListener('click', calculateRisk);
};