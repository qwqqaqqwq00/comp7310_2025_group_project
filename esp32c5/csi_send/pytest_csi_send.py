import pytest
import serial
import time
import csv

# 配置串口
SERIAL_PORT = 'COM6'  # 根据你的实际串口修改
BAUD_RATE = 115200

@pytest.fixture(scope="module")
def serial_connection():
    """Fixture to establish a serial connection."""
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # 等待设备初始化
    yield ser
    ser.close()

def test_send_data_from_csv(serial_connection):
    """Test sending data from the CSV file."""
    with open('..\\..\\benchmark\\breathing_rate\\evaluation\\CSI20250227_191018.csv', 'r') as f:  # 读取 CSV 文件
        data = f.readlines()
    for row in data:
        to_send = row

        serial_connection.write(to_send.encode('utf-8'))
        time.sleep(1)

        # response = serial_connection.read_all().decode('utf-8')
        # assert "ACK" in response 

    print("All data sent successfully.")