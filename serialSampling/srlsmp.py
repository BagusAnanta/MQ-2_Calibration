import serial
import csv


def getSample(serialPort : str = "/dev/ttyUSB0", nameFile : str = "data.csv", bautrate = 9600):
  ser = serial.Serial(serialPort, bautrate)

  with open(nameFile, 'w', newline='', encoding='utf-8') as file:
    writer = csv.writer(file)

    while True:
      line = ser.readline().decode().strip()
      print(line)

      data = line.split(",")

      if len(data) == 2:
        writer.writerow(data)

if __name__ == '__main__':
  # main at here
  getSample(nameFile = "dataR0New21Apr.csv")
