/**
 * ========================================
 * API.JS - API Communication
 * ========================================
 */

const checkApiHealth = async () => {
    try {
        const response = await fetch(`${CONFIG.API_BASE_URL}/health`, {
            method: 'GET',
            headers: { 'Content-Type': 'application/json' }
        });
        if (response.ok) {
            const data = await response.json();
            STATE.apiOnline = true;
            ELEMENTS.apiStatusIndicator.className = 'status-indicator status-online';
            ELEMENTS.apiStatusText.textContent = `API Connected (${data.cache_info.cached_assets} assets cached)`;
            ELEMENTS.calculateBtn.disabled = false;
        } else {
            throw new Error('API health check failed');
        }
    } catch (error) {
        STATE.apiOnline = false;
        ELEMENTS.apiStatusIndicator.className = 'status-indicator status-offline';
        ELEMENTS.apiStatusText.textContent = 'API Offline';
        ELEMENTS.calculateBtn.disabled = true;
    }
};

const fetchMarketData = async (tickers, forceRefresh = false) => {
    try {
        const response = await fetch(`${CONFIG.API_BASE_URL}/update_market_data`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                tickers: tickers,
                force_refresh: forceRefresh
            })
        });

        if (response.ok || response.status === 207) {
            const data = await response.json();
            
            // Update cache
            for (const [ticker, marketData] of Object.entries(data.updated)) {
                STATE.marketDataCache[ticker] = marketData;
            }
            
            return data;
        }
    } catch (error) {
        console.error('Error fetching market data:', error);
    }
    return null;
};