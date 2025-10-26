/**
 * ========================================
 * APP.JS - Application Initialization
 * ========================================
 */

const initializeApp = () => {
    // Setup event listeners
    setupPortfolioForm();
    setupMarketDataRefresh();
    setupRiskCalculation();
    
    // Initial render
    renderPortfolio();
    
    // Check API health immediately and set interval
    checkApiHealth();
    setInterval(checkApiHealth, CONFIG.API_HEALTH_CHECK_INTERVAL);
};

// Initialize when DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initializeApp);
} else {
    initializeApp();
}