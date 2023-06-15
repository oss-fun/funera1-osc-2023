port=${1:-8000}
uvicorn main:app --reload --host 0.0.0.0 --port $port
