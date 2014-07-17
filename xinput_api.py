from ctypes import *


class XInputStateEx(Structure):
    _fields_ = [
        ("eventCount", c_long),
        ("up", c_short, 1), ("down", c_short, 1), ("left", c_short, 1), ("right", c_short, 1), ("start", c_short, 1),
        ("back", c_short, 1), ("leftThumb", c_short, 1), ("rightThumb", c_short, 1), ("leftShoulder", c_short, 1),
        ("rightShoulder", c_short, 1), ("guideButton", c_short, 1), ("unknown", c_short, 1), ("aButton", c_short, 1),
        ("bButton", c_short, 1), ("xButton", c_short, 1), ("yButton", c_short, 1),
        ("leftTrigger", c_ubyte),
        ("rightTrigger", c_ubyte),
        ("thumbLX", c_short),
        ("thumbLY", c_short),
        ("thumbRX", c_short),
        ("thumbRY", c_short),
    ]


class XInputVibration(Structure):
    _fields_ = [
        ("leftMotorSpeed", c_ushort),
        ("rightMotorSpeed", c_ushort),
    ]


class XInputVibrationEx(Structure):
    _fields_ = [
        ("leftMotorSpeed", c_ushort),
        ("rightMotorSpeed", c_ushort),
        ("leftTriggerMotorSpeed", c_ushort),   # XB1 impulse triggers via
        ("rightTriggerMotorSpeed", c_ushort),  # our custom XInput wrapper
    ]


def main():
    # Load XInput API
    xinput_api = WinDLL("xinput1_3.dll")
    xinput_get = xinput_api[100]  # XInputGetStateEx

    try:
        xinput_set = xinput_api[104]  # XInputSetStateEx (ordinal 104 in our XB1 XInput wrapper)
        vibration_state = XInputVibration()
        has_impulse_triggers = True
    except AttributeError:
        xinput_set = xinput_api.XInputSetState
        vibration_state = XInputVibrationEx()
        has_impulse_triggers = False

    controller_num = 0  # zero based
    controller_state = XInputStateEx()

    while True:

        result = xinput_get(controller_num, byref(controller_state))
        controller_connected = True if result == 0 else False

        # Process input
        if controller_connected:

            if controller_state.guideButton:
                print("guideButton")
            if controller_state.aButton:
                print("aButton")
            if controller_state.bButton:
                print("bButton")
            if controller_state.xButton:
                print("xButton")
            if controller_state.yButton:
                print("yButton")

            if has_impulse_triggers:
                vibration_state.leftMotorSpeed = 0
                vibration_state.rightMotorSpeed = controller_state.leftTrigger * 100
                vibration_state.leftTriggerMotorSpeed = 0
                vibration_state.rightTriggerMotorSpeed = 0
            else:
                vibration_state.leftMotorSpeed = 0
                vibration_state.rightMotorSpeed = controller_state.leftTrigger * 100

            xinput_set(controller_num, byref(vibration_state))


if __name__ == "__main__":
    main()