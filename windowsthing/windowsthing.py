import asyncio
import serial
from serial.tools.list_ports import comports
from pystray import Icon as icon, Menu as menu, MenuItem as item
import threading
import time
import json
from PIL import Image
from winrt.windows.media.control import GlobalSystemMediaTransportControlsSessionManager as MediaManager
from winrt.windows.media.playback import MediaPlaybackState

running = True
ser = None

def format_mmss(seconds):
    minutes = int(seconds // 60)
    secs = int(seconds % 60)
    return f"{minutes}:{secs:02d}"

def load_port():
    try:
        with open("config.json", "r") as f:
            return json.load(f).get("port", "0")
    except FileNotFoundError:
        return "0"

def save_port(port):
    with open("config.json", "w") as f:
        json.dump({"port": port}, f)
        
def start_async_loop():
    asyncio.run(get_current_media())

def on_clicked(icon, item):
    global selectedport
    selectedport = item.text
    save_port(selectedport)
    connect_port()
    


def connect_port():
    global ser, selectedport

    try:
        if ser and ser.is_open:
            ser.close()

        ser = serial.Serial(selectedport, 9600, timeout=1)

    except serial.serialutil.SerialException as e:
        icon.notify("Error opening", selectedport)
        ser = None
        
def refresh_ports(icon, item=None):
    icon.menu = build_menu()
    icon.update_menu()
        
def on_exit(icon, item):
    global running
    running = False
    global ser
    if ser != None:
        try:
            if ser.is_open:
                ser.close()
        except serial.serialutil.SerialException:
            pass
    
    icon.stop()

availableports = comports()
port_names = [port.device for port in availableports]

image = Image.open("icon.ico")

def build_menu():
    ports = [port.device for port in comports()]

    return menu(
        item(
            "Ports",
            menu(
                *[item(name, on_clicked) for name in ports]
            )
        ),
        item("Refresh Ports", refresh_ports),
        item("Reconnect", connect_port),
        item("Exit", on_exit)
    )
    
icon = icon(
    "Windowsthing",
    image,
    menu=build_menu()
)

selectedport = load_port()

if selectedport != "0":
    connect_port()

        

async def get_current_media():
    manager = await MediaManager.request_async()
    track = None
    track_start_time = None
    position = None
    paused_position = None 
    while running:
        sessions = manager.get_sessions()
        if not sessions:
            if ser and ser.is_open:
                      try:
                          ser.write("No Media Playing|--|paused|0:00/0:00".encode("utf-8", errors="ignore"))
                      except serial.serialutil.SerialException:
                          icon.notify("COM Port Error")
                          selectedport = select_port()
        else:
            for session in sessions:
                info = await session.try_get_media_properties_async()
                playback_state = session.get_playback_info().playback_status
                playback = None
                timeline = session.get_timeline_properties()
                
                duration_sec = timeline.end_time.duration / 10_000_000
                reported_position_sec = timeline.position.duration /10_000_000
                duration_str = format_mmss(duration_sec)


            
                if track != info.title:
                    track_start_time = int(time.time())
                    position = 0
                else:
                    if playback_state == 4:
                        if position < duration_sec:
                            position = int(time.time()) - track_start_time
                        else:
                            position = 0

                if reported_position_sec == duration_sec or reported_position_sec < 1 or int(reported_position_sec) == paused_position and playback_state == 4: 
                    position_str = format_mmss(position)
                    
                else:
                    position_str = format_mmss(reported_position_sec)

                


                playprog = f"{position_str}/{duration_str}"
                
                
                if playback_state == 4:
                    playback = "playing"
                elif playback_state == 5:
                    playback = "paused"
                    paused_position = int(reported_position_sec)
                if info:
                        data = [
                            info.title or "",
                            info.artist or "",
                            playback,
                            playprog
                        ]

                payload = "|".join(data) + "\n"


                if ser and ser.is_open:
                      try:
                          ser.write(payload.encode("utf-8", errors="ignore"))
                      except serial.serialutil.SerialException:
                          icon.notify("COM Port Error")
                          selectedport = select_port()

                track = info.title

        await asyncio.sleep(2)

if __name__ == "__main__":
    threading.Thread(
        target=start_async_loop,
        daemon=True
    ).start()
    icon.run()
