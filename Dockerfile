FROM python:3.10-slim
WORKDIR /workspace
RUN pip install -U platformio
CMD ["pio", "run", "-e", "esp32s3_devkit"]