import RemoteClient
import time

test = RemoteClient.RemoteClient()
time.sleep(1)
test.panel_emulator('black')
time.sleep(1)
test.panel_emulator('red')
time.sleep(1)
test.panel_emulator('purple')
time.sleep(1)
test.panel_emulator('orange')