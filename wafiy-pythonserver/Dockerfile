FROM python:3.10-slim

WORKDIR /app

COPY wafiyserver.py .

RUN pip install --no-cache-dir mysql-connector-python

EXPOSE 8080

CMD ["python", "wafiyserver.py"]
