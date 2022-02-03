import requests
import time

def setEnabled(enabled):
  r = requests.post('http://rekuperace/a/t/', json={"duration": "10000","on":enabled, "power":0}, headers={"content-type": "application/json"})
  print("result is " + str(r.status_code))

while True:
  print("enabling manual")
  setEnabled(True)
  print("waiting until next execution..")
  time.sleep(900)


