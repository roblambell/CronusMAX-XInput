# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'dialog.ui'
#
# Created: Thu Jun  5 23:39:00 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.setWindowModality(QtCore.Qt.NonModal)
        Dialog.resize(378, 335)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(Dialog.sizePolicy().hasHeightForWidth())
        Dialog.setSizePolicy(sizePolicy)
        Dialog.setMinimumSize(QtCore.QSize(378, 335))
        Dialog.setMaximumSize(QtCore.QSize(378, 335))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("app.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        Dialog.setWindowIcon(icon)
        self.tabWidget = QtGui.QTabWidget(Dialog)
        self.tabWidget.setGeometry(QtCore.QRect(10, 10, 361, 301))
        self.tabWidget.setTabShape(QtGui.QTabWidget.Rounded)
        self.tabWidget.setObjectName("tabWidget")
        self.tab_DeviceMonitor = QtGui.QWidget()
        self.tab_DeviceMonitor.setObjectName("tab_DeviceMonitor")
        self.tableWidget_Activity = QtGui.QTableWidget(self.tab_DeviceMonitor)
        self.tableWidget_Activity.setGeometry(QtCore.QRect(10, 10, 331, 251))
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.tableWidget_Activity.sizePolicy().hasHeightForWidth())
        self.tableWidget_Activity.setSizePolicy(sizePolicy)
        self.tableWidget_Activity.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
        self.tableWidget_Activity.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers)
        self.tableWidget_Activity.setSelectionMode(QtGui.QAbstractItemView.NoSelection)
        self.tableWidget_Activity.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)
        self.tableWidget_Activity.setShowGrid(True)
        self.tableWidget_Activity.setRowCount(21)
        self.tableWidget_Activity.setObjectName("tableWidget_Activity")
        self.tableWidget_Activity.setColumnCount(3)
        self.tableWidget_Activity.setRowCount(21)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_Activity.setHorizontalHeaderItem(0, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_Activity.setHorizontalHeaderItem(1, item)
        item = QtGui.QTableWidgetItem()
        self.tableWidget_Activity.setHorizontalHeaderItem(2, item)
        self.tableWidget_Activity.horizontalHeader().setDefaultSectionSize(70)
        self.tableWidget_Activity.horizontalHeader().setMinimumSectionSize(33)
        self.tableWidget_Activity.verticalHeader().setVisible(False)
        self.tableWidget_Activity.verticalHeader().setDefaultSectionSize(20)
        self.tabWidget.addTab(self.tab_DeviceMonitor, "")
        self.tab_Options = QtGui.QWidget()
        self.tab_Options.setObjectName("tab_Options")
        self.groupBox_Controller = QtGui.QGroupBox(self.tab_Options)
        self.groupBox_Controller.setGeometry(QtCore.QRect(10, 10, 331, 81))
        self.groupBox_Controller.setObjectName("groupBox_Controller")
        self.radioButton_PS3 = QtGui.QRadioButton(self.groupBox_Controller)
        self.radioButton_PS3.setGeometry(QtCore.QRect(190, 20, 41, 17))
        self.radioButton_PS3.setObjectName("radioButton_PS3")
        self.radioButton_XB1 = QtGui.QRadioButton(self.groupBox_Controller)
        self.radioButton_XB1.setGeometry(QtCore.QRect(100, 20, 71, 17))
        self.radioButton_XB1.setObjectName("radioButton_XB1")
        self.radioButton_X360CE = QtGui.QRadioButton(self.groupBox_Controller)
        self.radioButton_X360CE.setGeometry(QtCore.QRect(250, 20, 61, 17))
        self.radioButton_X360CE.setObjectName("radioButton_X360CE")
        self.radioButton_X360 = QtGui.QRadioButton(self.groupBox_Controller)
        self.radioButton_X360.setGeometry(QtCore.QRect(10, 20, 71, 17))
        self.radioButton_X360.setObjectName("radioButton_X360")
        self.label_Instructions = QtGui.QLabel(self.groupBox_Controller)
        self.label_Instructions.setGeometry(QtCore.QRect(10, 40, 321, 31))
        self.label_Instructions.setOpenExternalLinks(True)
        self.label_Instructions.setObjectName("label_Instructions")
        self.groupBox_Scripting = QtGui.QGroupBox(self.tab_Options)
        self.groupBox_Scripting.setGeometry(QtCore.QRect(10, 100, 331, 51))
        self.groupBox_Scripting.setObjectName("groupBox_Scripting")
        self.pushButton_EditScript = QtGui.QPushButton(self.groupBox_Scripting)
        self.pushButton_EditScript.setGeometry(QtCore.QRect(10, 20, 111, 23))
        self.pushButton_EditScript.setObjectName("pushButton_EditScript")
        self.label_Instructions_2 = QtGui.QLabel(self.groupBox_Scripting)
        self.label_Instructions_2.setGeometry(QtCore.QRect(130, 10, 191, 41))
        self.label_Instructions_2.setWordWrap(True)
        self.label_Instructions_2.setObjectName("label_Instructions_2")
        self.groupBox_Configurations = QtGui.QGroupBox(self.tab_Options)
        self.groupBox_Configurations.setGeometry(QtCore.QRect(10, 160, 331, 51))
        self.groupBox_Configurations.setObjectName("groupBox_Configurations")
        self.checkBox_Passthrough = QtGui.QCheckBox(self.groupBox_Configurations)
        self.checkBox_Passthrough.setGeometry(QtCore.QRect(10, 20, 311, 17))
        self.checkBox_Passthrough.setObjectName("checkBox_Passthrough")
        self.pushButton_SaveAndRestart = QtGui.QPushButton(self.tab_Options)
        self.pushButton_SaveAndRestart.setGeometry(QtCore.QRect(80, 240, 201, 23))
        self.pushButton_SaveAndRestart.setObjectName("pushButton_SaveAndRestart")
        self.label_Warning = QtGui.QLabel(self.tab_Options)
        self.label_Warning.setGeometry(QtCore.QRect(40, 220, 281, 20))
        self.label_Warning.setTextFormat(QtCore.Qt.AutoText)
        self.label_Warning.setAlignment(QtCore.Qt.AlignCenter)
        self.label_Warning.setObjectName("label_Warning")
        self.tabWidget.addTab(self.tab_Options, "")
        self.tab_About = QtGui.QWidget()
        self.tab_About.setObjectName("tab_About")
        self.label_About = QtGui.QLabel(self.tab_About)
        self.label_About.setGeometry(QtCore.QRect(10, 10, 331, 261))
        self.label_About.setWordWrap(True)
        self.label_About.setOpenExternalLinks(True)
        self.label_About.setObjectName("label_About")
        self.tabWidget.addTab(self.tab_About, "")
        self.label_Status = QtGui.QLabel(Dialog)
        self.label_Status.setGeometry(QtCore.QRect(13, 310, 361, 21))
        self.label_Status.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.label_Status.setObjectName("label_Status")

        self.retranslateUi(Dialog)
        self.tabWidget.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QtGui.QApplication.translate("Dialog", "CronusMAX-XInput", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_Activity.horizontalHeaderItem(0).setText(QtGui.QApplication.translate("Dialog", "Button", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_Activity.horizontalHeaderItem(1).setText(QtGui.QApplication.translate("Dialog", "Input", None, QtGui.QApplication.UnicodeUTF8))
        self.tableWidget_Activity.horizontalHeaderItem(2).setText(QtGui.QApplication.translate("Dialog", "Output", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_DeviceMonitor), QtGui.QApplication.translate("Dialog", "Device Monitor", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_Controller.setTitle(QtGui.QApplication.translate("Dialog", "Input Controller", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_PS3.setText(QtGui.QApplication.translate("Dialog", "PS3", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_XB1.setText(QtGui.QApplication.translate("Dialog", "Xbox One", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_X360CE.setText(QtGui.QApplication.translate("Dialog", "x360ce", None, QtGui.QApplication.UnicodeUTF8))
        self.radioButton_X360.setText(QtGui.QApplication.translate("Dialog", "Xbox 360", None, QtGui.QApplication.UnicodeUTF8))
        self.label_Instructions.setText(QtGui.QApplication.translate("Dialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">For Xbox One install the included driver for your controller.<br />For DirectInput peripherals: configure them in x360ce first.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_Scripting.setTitle(QtGui.QApplication.translate("Dialog", "GPC Scripting", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_EditScript.setText(QtGui.QApplication.translate("Dialog", "Edit GPC Script", None, QtGui.QApplication.UnicodeUTF8))
        self.label_Instructions_2.setText(QtGui.QApplication.translate("Dialog", "<html><head/><body><p>Scripting via a C interpreter. Not all GPC language features are supported</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_Configurations.setTitle(QtGui.QApplication.translate("Dialog", "Configurations", None, QtGui.QApplication.UnicodeUTF8))
        self.checkBox_Passthrough.setText(QtGui.QApplication.translate("Dialog", "Passthrough input from authentication controller", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_SaveAndRestart.setText(QtGui.QApplication.translate("Dialog", "Save Options && Restart Forwarding", None, QtGui.QApplication.UnicodeUTF8))
        self.label_Warning.setText(QtGui.QApplication.translate("Dialog", "<html><head/><body><p style=\"color:#800000\">Input forwarding will be momentarily interrupted.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_Options), QtGui.QApplication.translate("Dialog", "Options", None, QtGui.QApplication.UnicodeUTF8))
        self.label_About.setText(QtGui.QApplication.translate("Dialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">CronusMAX-XInput</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">XInput for GPP/CronusMAX device</p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Author</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Misdari of ControllerMAX Forums</p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Support</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://controllermax.com/forums\"><span style=\" text-decoration: underline; color:#0000ff;\">controllermax.com/forums</span></a></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Source Code</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://github.com/badgio/CronusMAX-XInput\"><span style=\" text-decoration: underline; color:#0000ff;\">github.com/badgio/CronusMAX-XInput</span></a></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">xinput1_3.dll © Microsoft. xinput1_3_xb1.dll © Misadri. xinput1_3_ps3.dll © Scarlet.Crush. picoc © Zik Saleeba. Notepad2 © Florian Balmer. x360ce © Racer_S, Robert Krawczyk</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_About), QtGui.QApplication.translate("Dialog", "About", None, QtGui.QApplication.UnicodeUTF8))
        self.label_Status.setText(QtGui.QApplication.translate("Dialog", "Status message goes here", None, QtGui.QApplication.UnicodeUTF8))

