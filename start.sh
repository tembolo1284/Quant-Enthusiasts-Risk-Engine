#!/bin/bash
# start.sh - Launch Quant Enthusiasts Risk Engine (Python API + JS Dashboard)

set -e  # Exit immediately if a command fails

echo "Starting Quant Enthusiasts Risk Engine..."

# -----------------------------
# 1. Python API
# -----------------------------
echo "Setting up Python API..."
cd python_api

# Create virtual environment if it doesn't exist
if [ ! -d "venv" ]; then
    echo "Creating Python virtual environment..."
    python -m venv venv
fi

# Activate virtual environment
source venv/bin/activate

# Install dependencies
pip install --upgrade pip
pip install -r requirements.txt

# Build Python bindings
python setup.py build_ext --inplace

# Start Python API in background
echo "Launching Python API..."
python app.py &
API_PID=$!

cd ..

# -----------------------------
# 2. JS Dashboard
# -----------------------------
echo "Setting up JS Dashboard..."
cd js_dashboard

# Install Node dependencies if package.json exists
if [ -f "package.json" ]; then
    npm install
fi

# Start local server
echo "Launching JS Dashboard..."
npx serve . -p 3000 &
DASHBOARD_PID=$!

cd ..

# -----------------------------
# 3. Completion message
# -----------------------------
echo "Quant Enthusiasts Risk Engine started successfully!"
echo "Python API running at http://127.0.0.1:5000"
echo "JS Dashboard running at http://127.0.0.1:3000"
echo "Press Ctrl+C to stop."

# Wait for background processes
wait $API_PID $DASHBOARD_PID
