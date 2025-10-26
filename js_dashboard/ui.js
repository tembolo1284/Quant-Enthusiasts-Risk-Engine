/**
 * ========================================
 * UI.JS - DOM Elements & UI Utilities
 * ========================================
 */

const ELEMENTS = {
    instrumentForm: document.getElementById('instrument-form'),
    assetIdInput: document.getElementById('asset-id'),
    styleSelect: document.getElementById('style'),
    typeSelect: document.getElementById('type'),
    strikeInput: document.getElementById('strike'),
    expiryInput: document.getElementById('expiry'),
    quantityInput: document.getElementById('quantity'),
    portfolioTable: document.getElementById('portfolio-table'),
    emptyPortfolio: document.getElementById('empty-portfolio'),
    portfolioCount: document.getElementById('portfolio-count'),
    marketDataDisplay: document.getElementById('market-data-display'),
    refreshMarketDataBtn: document.getElementById('refresh-market-data-btn'),
    calculateBtn: document.getElementById('calculate-risk-btn'),
    resultsContainer: document.getElementById('results-container'),
    autoFetchInfo: document.getElementById('auto-fetch-info'),
    autoFetchedAssets: document.getElementById('auto-fetched-assets'),
    spinner: document.getElementById('spinner'),
    errorMessage: document.getElementById('error-message'),
    initialState: document.getElementById('initial-state'),
    apiStatusIndicator: document.getElementById('api-status-indicator'),
    apiStatusText: document.getElementById('api-status-text')
};

const showError = (message) => {
    ELEMENTS.errorMessage.textContent = message;
    ELEMENTS.errorMessage.classList.remove('hidden');
    ELEMENTS.resultsContainer.classList.add('hidden');
    ELEMENTS.autoFetchInfo.classList.add('hidden');
    ELEMENTS.initialState.classList.add('hidden');
    setTimeout(() => ELEMENTS.errorMessage.classList.add('hidden'), 10000);
};

const showSpinner = () => {
    ELEMENTS.spinner.classList.remove('hidden');
    ELEMENTS.resultsContainer.classList.add('hidden');
    ELEMENTS.errorMessage.classList.add('hidden');
    ELEMENTS.autoFetchInfo.classList.add('hidden');
    ELEMENTS.initialState.classList.add('hidden');
};

const hideSpinner = () => {
    ELEMENTS.spinner.classList.add('hidden');
};

const formatValue = (value, decimals = 2) => {
    if (value === null || value === undefined || isNaN(value)) return 'N/A';
    return value.toLocaleString('en-US', {
        minimumFractionDigits: decimals,
        maximumFractionDigits: decimals
    });
};

const getMetricColor = (label, value) => {
    if (label.includes('VaR')) return 'text-red-400';
    if (label === 'Total PV') return value >= 0 ? 'text-green-400' : 'text-red-400';
    if (label === 'Delta') return Math.abs(value) > 10 ? 'text-yellow-400' : 'text-cyan-400';
    return 'text-cyan-400';
};