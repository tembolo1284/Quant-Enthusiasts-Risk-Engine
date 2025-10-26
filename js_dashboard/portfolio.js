/**
 * ========================================
 * PORTFOLIO.JS - Portfolio Management
 * ========================================
 */

const updatePortfolioCount = () => {
    const count = STATE.portfolio.length;
    ELEMENTS.portfolioCount.textContent = `${count} instrument${count !== 1 ? 's' : ''}`;
};

const updateMarketDataDisplay = () => {
    const assets = new Set(STATE.portfolio.map(p => p.asset_id));

    if (assets.size === 0) {
        ELEMENTS.marketDataDisplay.innerHTML = '<p class="text-gray-400 text-center py-8">Add instruments to view market data</p>';
        return;
    }

    ELEMENTS.marketDataDisplay.innerHTML = '';

    assets.forEach(asset => {
        const cached = STATE.marketDataCache[asset];
        const card = document.createElement('div');
        card.className = 'p-3 bg-gray-800/40 rounded-lg border border-gray-700/60';

        if (cached) {
            const ageMinutes = Math.floor((new Date() - new Date(cached.last_updated)) / 60000);
            const ageText = ageMinutes < 60 ? `${ageMinutes}m ago` : `${Math.floor(ageMinutes / 60)}h ago`;

            card.innerHTML = `
                <div class="flex justify-between items-start mb-2">
                    <h4 class="font-bold text-cyan-400">${asset}</h4>
                    <span class="text-xs text-gray-500">${ageText}</span>
                </div>
                <div class="grid grid-cols-2 gap-2 text-xs">
                    <div><span class="text-gray-400">Spot:</span> <span class="text-white">${cached.spot.toFixed(2)}</span></div>
                    <div><span class="text-gray-400">Vol:</span> <span class="text-white">${(cached.vol * 100).toFixed(1)}%</span></div>
                    <div><span class="text-gray-400">Rate:</span> <span class="text-white">${(cached.rate * 100).toFixed(2)}%</span></div>
                    <div><span class="text-gray-400">Div:</span> <span class="text-white">${(cached.dividend * 100).toFixed(2)}%</span></div>
                </div>
            `;
        } else {
            card.innerHTML = `
                <h4 class="font-bold text-cyan-400 mb-2">${asset}</h4>
                <p class="text-xs text-yellow-400">⚠ Will fetch on calculate</p>
            `;
        }

        ELEMENTS.marketDataDisplay.appendChild(card);
    });
};

const renderPortfolio = () => {
    ELEMENTS.portfolioTable.innerHTML = '';
    if (STATE.portfolio.length === 0) {
        ELEMENTS.emptyPortfolio.classList.remove('hidden');
        ELEMENTS.calculateBtn.disabled = true;
        updatePortfolioCount();
        updateMarketDataDisplay();
        return;
    }
    ELEMENTS.emptyPortfolio.classList.add('hidden');
    ELEMENTS.calculateBtn.disabled = !STATE.apiOnline;

    STATE.portfolio.forEach((instrument, index) => {
        const row = document.createElement('tr');
        row.className = 'hover:bg-gray-700/50 transition fade-in';
        const qtyColor = instrument.quantity > 0 ? 'text-green-400' : 'text-red-400';
        const qtySign = instrument.quantity > 0 ? '+' : '';
        row.innerHTML = `
            <td class="py-2 md:py-3 px-2 md:px-4 font-medium text-cyan-400 text-xs md:text-sm">${instrument.asset_id}</td>
            <td class="py-2 md:py-3 px-2 md:px-4">
                <span class="px-2 py-1 text-xs rounded-full bg-purple-500/20 text-purple-300">
                    ${instrument.style}
                </span>
            </td>
            <td class="py-2 md:py-3 px-2 md:px-4">
                <span class="px-2 py-1 text-xs rounded-full ${instrument.type === 'call' ? 'bg-green-500/20 text-green-300' : 'bg-red-500/20 text-red-300'}">
                    ${instrument.type}
                </span>
            </td>
            <td class="py-2 md:py-3 px-2 md:px-4 text-right font-mono text-xs md:text-sm ${qtyColor}">${qtySign}${instrument.quantity}</td>
            <td class="py-2 md:py-3 px-2 md:px-4 text-right font-mono text-xs md:text-sm">${instrument.strike.toFixed(2)}</td>
            <td class="py-2 md:py-3 px-2 md:px-4 text-right font-mono text-xs md:text-sm">${instrument.expiry.toFixed(2)}y</td>
            <td class="py-2 md:py-3 px-2 md:px-4 text-center">
                <button onclick="removeInstrument(${index})"
                        class="text-gray-400 hover:text-red-400 hover:bg-red-500/20 rounded-full w-6 h-6 md:w-7 md:h-7 flex items-center justify-center font-bold text-lg mx-auto"
                        title="Remove instrument">
                    ×
                </button>
            </td>
        `;
        ELEMENTS.portfolioTable.appendChild(row);
    });
    updatePortfolioCount();
    updateMarketDataDisplay();
};

window.removeInstrument = (index) => {
    if (confirm(`Remove ${STATE.portfolio[index].asset_id} ${STATE.portfolio[index].type} from portfolio?`)) {
        STATE.portfolio.splice(index, 1);
        renderPortfolio();
        ELEMENTS.resultsContainer.classList.add('hidden');
        ELEMENTS.autoFetchInfo.classList.add('hidden');
        ELEMENTS.initialState.classList.remove('hidden');
    }
};

const setupPortfolioForm = () => {
    ELEMENTS.instrumentForm.addEventListener('submit', (e) => {
        e.preventDefault();
        const newInstrument = {
            asset_id: ELEMENTS.assetIdInput.value.trim().toUpperCase(),
            style: ELEMENTS.styleSelect.value.toLowerCase(),
            type: ELEMENTS.typeSelect.value.toLowerCase(),
            strike: parseFloat(ELEMENTS.strikeInput.value),
            expiry: parseFloat(ELEMENTS.expiryInput.value),
            quantity: parseInt(ELEMENTS.quantityInput.value)
        };

        STATE.portfolio.push(newInstrument);
        renderPortfolio();

        ELEMENTS.instrumentForm.reset();
        ELEMENTS.assetIdInput.value = newInstrument.asset_id;
        ELEMENTS.strikeInput.value = "100";
        ELEMENTS.expiryInput.value = "1.0";
        ELEMENTS.quantityInput.value = "100";
    });
}; const setupMarketDataRefresh = () => {
    ELEMENTS.refreshMarketDataBtn.addEventListener('click', async () => {
        const assets = [...new Set(STATE.portfolio.map(p => p.asset_id))];
        if (assets.length === 0) return; ELEMENTS.refreshMarketDataBtn.disabled = true;
        ELEMENTS.refreshMarketDataBtn.textContent = '⏳ Fetching...';

        await fetchMarketData(assets, true);
        updateMarketDataDisplay();
        await checkApiHealth();

        ELEMENTS.refreshMarketDataBtn.disabled = false;
        ELEMENTS.refreshMarketDataBtn.textContent = '🔄 Refresh';
    });
};