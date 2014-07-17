import sys
import subprocess
import time
from dialog import Ui_Dialog
from PySide import QtGui
from PySide.QtCore import Qt, QThread, Signal
from xinput_api import *


class ForwarderThread(QThread):
    updated = Signal(object)
    stop_pending = False

    def stop(self):
        self.stop_pending = True

    def run(self):
        millis = lambda: int(round(time.time() * 1000))
        ui_report_timer = millis()
        ui_report = {}

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

        passthru_input = False

        # report.input and XInput get merged as per config
        xinput_input = [None] * 21
        merged_input = [None] * 21
        output = [None] * 21

        while self.stop_pending is False:

            result = xinput_get(controller_num, byref(controller_state))
            controller_connected = True if result == 0 else False
            device_connected = False  # TODO: CM API

            # Process input
            if controller_connected:

                # [0 ~ 255] to [0 ~ 100] and [-32767 ~ 32767] to [-100 ~ 100]
                lt = round((controller_state.leftTrigger / 255) * 100)
                rt = round((controller_state.rightTrigger / 255) * 100)
                lx = round((controller_state.thumbLX / 32767) * 100)
                ly = round((controller_state.thumbLY / 32767) * 100) * -1  # CM expects Y-axis -100 up, 100 down
                rx = round((controller_state.thumbRX / 32767) * 100)
                ry = round((controller_state.thumbRY / 32767) * 100) * -1  # CM expects Y-axis -100 up, 100 down

                xinput_input[0] = 100 if controller_state.guideButton else 0  # Guide
                xinput_input[1] = 100 if controller_state.back else 0  # Back
                xinput_input[2] = 100 if controller_state.start else 0  # Start
                xinput_input[3] = 100 if controller_state.rightShoulder else 0  # Right Shoulder
                xinput_input[4] = rt  # Right Trigger [0 ~ 100] %
                xinput_input[5] = 100 if controller_state.rightThumb else 0  # Right Analog Stick (Pressed)
                xinput_input[6] = 100 if controller_state.leftShoulder else 0  # Left Shoulder
                xinput_input[7] = lt  # Left Trigger [0 ~ 100] %
                xinput_input[8] = 100 if controller_state.leftThumb else 0  # Left Analog Stick (Pressed)
                xinput_input[9] = rx  # Right Analog Stick X-axis [-100 ~ 100] %
                xinput_input[10] = ry  # Right Analog Stick Y-axis [-100 ~ 100] %
                xinput_input[11] = lx  # Left Analog Stick X-axis [-100 ~ 100] %
                xinput_input[12] = ly  # Left Analog Stick Y-axis [-100 ~ 100] %
                xinput_input[13] = 100 if controller_state.up else 0  # DPad Up
                xinput_input[14] = 100 if controller_state.down else 0  # DPad Down
                xinput_input[15] = 100 if controller_state.left else 0  # DPad Left
                xinput_input[16] = 100 if controller_state.right else 0  # DPad Right
                xinput_input[17] = 100 if controller_state.yButton else 0  # Y
                xinput_input[18] = 100 if controller_state.bButton else 0  # B
                xinput_input[19] = 100 if controller_state.aButton else 0  # A
                xinput_input[20] = 100 if controller_state.xButton else 0  # X

                #if device_connected:
                #    read cm report
                report_input = xinput_input

                # Merge or disregard CM input data (auth controller)
                if passthru_input and device_connected:
                    for x in range(0, 21):
                        if abs(report_input[x]) > abs(xinput_input[x]):
                            merged_input[x] = report_input[x]
                        else:
                            merged_input[x] = xinput_input[x]
                else:
                    for x in range(0, 21):
                        merged_input[x] = xinput_input[x]

            elif device_connected:
                # read cm report
                for x in range(0, 21):
                    merged_input[x] = 0

            # TODO: rumble from console
            # TODO: GPC interpreter
            # rumble to controller
            if controller_connected:
                if has_impulse_triggers:
                    vibration_state.leftMotorSpeed = 0
                    vibration_state.rightMotorSpeed = controller_state.leftTrigger * 100
                    vibration_state.leftTriggerMotorSpeed = 0
                    vibration_state.rightTriggerMotorSpeed = 0
                else:
                    vibration_state.leftMotorSpeed = 0
                    vibration_state.rightMotorSpeed = controller_state.leftTrigger * 100

                xinput_set(controller_num, byref(vibration_state))

            # TODO: output to console
            for x in range(0, 21):
                output[x] = merged_input[x]

            ui_report['controller_connected'] = controller_connected
            ui_report['device_connected'] = device_connected
            ui_report['merged_input'] = merged_input
            ui_report['output'] = output
            ui_report['rumble_in'] = 0
            ui_report['rumble_out'] = 0

            # Report to UI - wait at least 100ms between reports
            if (millis() - ui_report_timer) > 100:
                self.updated.emit(ui_report)
                ui_report_timer = millis()

        # Free resources and unload libraries
        #windll.FreeLibrary(xinput_api)


class Main(QtGui.QMainWindow):

    def __init__(self, parent=None):
        super(Main, self).__init__(parent)
        self.ui = Ui_Dialog()
        self.ui.setupUi(self)
        self.ui.pushButton_EditScript.clicked.connect(self.open_script_editor)
        self.ui.pushButton_SaveAndRestart.clicked.connect(self.restart_forwarder)

        self.ui.tableWidget_Activity.setColumnWidth(0, 180)
        self.ui.tableWidget_Activity.setColumnWidth(1, 65)
        self.ui.tableWidget_Activity.setColumnWidth(2, 65)

        self.button_label = [None] * 21
        self.button_label[0] = "Guide"
        self.button_label[1] = "Back"
        self.button_label[2] = "Start"
        self.button_label[3] = "Right Shoulder"
        self.button_label[4] = "Right Trigger"
        self.button_label[5] = "Right Analog Stick (Pressed)"
        self.button_label[6] = "Left Shoulder"
        self.button_label[7] = "Left Trigger"
        self.button_label[8] = "Left Analog Stick (Pressed)"
        self.button_label[9] = "Right Analog Stick X-axis"
        self.button_label[10] = "Right Analog Stick Y-axis"
        self.button_label[11] = "Left Analog Stick X-axis"
        self.button_label[12] = "Left Analog Stick Y-axis"
        self.button_label[13] = "DPad Up"
        self.button_label[14] = "DPad Down"
        self.button_label[15] = "DPad Left"
        self.button_label[16] = "DPad Right"
        self.button_label[17] = "Y"
        self.button_label[18] = "B"
        self.button_label[19] = "A"
        self.button_label[20] = "X"

        self.thread = False
        self.start_forwarder()

    def get_data(self, ui_report):
        merged_input = ui_report['merged_input']
        output = ui_report['output']

        self.ui.tableWidget_Activity.clearContents()
        current_row = 0

        for x in range(0, 21):
            if merged_input[x] is not 0 or output[x] is not 0:
                input_val = str(merged_input[x]) if merged_input[x] < 0 else "+" + str(merged_input[x])
                output_val = str(output[x]) if output[x] < 0 else "+" + str(output[x])
                self.ui.tableWidget_Activity.setItem(current_row, 0, QtGui.QTableWidgetItem(self.button_label[x]))
                self.ui.tableWidget_Activity.setItem(current_row, 1, QtGui.QTableWidgetItem(input_val))
                self.ui.tableWidget_Activity.setItem(current_row, 2, QtGui.QTableWidgetItem(output_val))
                current_row += 1

        if ui_report['device_connected'] is False and ui_report['controller_connected'] is False:
            self.ui.label_Status.setText("Waiting for controller and device")
        elif ui_report['device_connected'] is True and ui_report['controller_connected'] is False:
            self.ui.label_Status.setText("Waiting for controller")
        elif ui_report['device_connected'] is False and ui_report['controller_connected'] is True:
            self.ui.label_Status.setText("Waiting for device")
        else:
            self.ui.label_Status.setText("Ready")

    @staticmethod
    def open_script_editor():
        if sys.platform == "linux" or sys.platform == "linux2":
            # Linux
            subprocess.Popen(['gedit', 'XInput.gpc'])
        elif sys.platform == "darwin":
            # OS X
            subprocess.Popen(['open -a TextEdit', 'XInput.gpc'])
        elif sys.platform == "win32":
            # Windows
            subprocess.Popen(['Notepad2\\notepad2.exe', 'XInput.gpc'])

    def start_forwarder(self):
        self.thread = ForwarderThread()
        self.thread.updated.connect(self.get_data, Qt.QueuedConnection)
        self.thread.start()

    def stop_forwarder(self):
        self.thread.stop()
        self.thread.wait()

    def restart_forwarder(self):
        self.stop_forwarder()
        self.start_forwarder()

    def closeEvent(self, event):
        self.stop_forwarder()
        event.accept()


if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = Main()
    window.show()
    sys.exit(app.exec_())