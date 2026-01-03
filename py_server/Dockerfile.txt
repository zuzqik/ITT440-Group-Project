# Use official Python image
FROM python:3.10-slim

# Set working directory inside container
WORKDIR /app

# Copy server code
COPY server.py .

# Install dependencies
RUN pip install mysql-connector-python

# Expose port (matches server)
EXPOSE 5002

# Command to run the server
CMD ["python", "server.py"]
