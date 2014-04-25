#pragma once

namespace CronusMAX_XInput {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Diagnostics;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			InitializeBackgoundWorker();

			int controllerNum = 0; // zero based
			backgroundWorker1->RunWorkerAsync(controllerNum);
		}


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;
	private: System::Windows::Forms::NotifyIcon^  notifyIcon;
	private: System::Windows::Forms::ContextMenuStrip^  notifyContextMenu;
	private: System::Windows::Forms::ToolStripMenuItem^  miToggleVisible;
	private: System::Windows::Forms::ToolStripMenuItem^  miExit;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;


	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ColumnHeader^  columnHeader3;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;

	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::GroupBox^  groupBox1;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::LinkLabel^  linkLabel1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::LinkLabel^  linkLabel2;
	private: System::Windows::Forms::Label^  Author;
	private: System::Windows::Forms::RadioButton^  radioButton4;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::CheckBox^  checkBox1;


	private: static bool isProcessingBackgroundWorkerReport = false;

	private:

		// Set up the BackgroundWorker object by  
		// attaching event handlers.  
		void InitializeBackgoundWorker()
		{
			backgroundWorker1->DoWork += gcnew DoWorkEventHandler( this, &Form1::backgroundWorker1_DoWork );
			//backgroundWorker1->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler( this, &Form1::backgroundWorker1_RunWorkerCompleted );
			backgroundWorker1->ProgressChanged += gcnew ProgressChangedEventHandler( this, &Form1::backgroundWorker1_ProgressChanged );
		}

		void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
		{
			// Get the BackgroundWorker that raised this event.
			BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
			XInputForwarder( safe_cast<Int32>(e->Argument), worker, e );
		}

		// This event handler updates the progress bar. 
		void backgroundWorker1_ProgressChanged( Object^ /*sender*/, ProgressChangedEventArgs^ e )
		{
			if( !isProcessingBackgroundWorkerReport )
			{
				isProcessingBackgroundWorkerReport = true;

				FORWARDER_STATE forwarderState = (FORWARDER_STATE)e->UserState;

				if(forwarderState.errorMessage)
				{
					this->toolStripStatusLabel1->Text = forwarderState.errorMessage;
				}
				else
				{
					// Update status
					if(!forwarderState.deviceConnected && !forwarderState.controllerConnected)
					{
						this->toolStripStatusLabel1->Text = "Waiting for controller and device";
					}
					else if(forwarderState.deviceConnected && !forwarderState.controllerConnected)
					{
						this->toolStripStatusLabel1->Text = "Waiting for controller";
					}
					else if(!forwarderState.deviceConnected && forwarderState.controllerConnected)
					{
						this->toolStripStatusLabel1->Text = "Waiting for device";
					}
					else
					{
						this->toolStripStatusLabel1->Text = "Ready";
					}

					// Check output console and set button labels
					array<System::String^> ^buttonLabel = gcnew array<System::String^>(21);
					buttonLabel[0] = "Guide";
					buttonLabel[1] = "Back";
					buttonLabel[2] = "Start";
					buttonLabel[3] = "Right Shoulder";
					buttonLabel[4] = "Right Trigger";
					buttonLabel[5] = "Right Analog Stick (Pressed)";
					buttonLabel[6] = "Left Shoulder";
					buttonLabel[7] = "Left Trigger";
					buttonLabel[8] = "Left Analog Stick (Pressed)";
					buttonLabel[9] = "Right Analog Stick X-axis";
					buttonLabel[10] = "Right Analog Stick Y-axis";
					buttonLabel[11] = "Left Analog Stick X-axis";
					buttonLabel[12] = "Left Analog Stick Y-axis";
					buttonLabel[13] = "DPad Up";
					buttonLabel[14] = "DPad Down";
					buttonLabel[15] = "DPad Left";
					buttonLabel[16] = "DPad Right";
					buttonLabel[17] = "Y";
					buttonLabel[18] = "B";
					buttonLabel[19] = "A";
					buttonLabel[20] = "X";

					array<System::String^> ^rumbleLabel = gcnew array<System::String^>(4);
					rumbleLabel[0] = "Left Motor (Rumble)";
					rumbleLabel[1] = "Right Motor (Rumble)";
					rumbleLabel[2] = "Left Trigger (Rumble)";
					rumbleLabel[3] = "Right Trigger (Rumble)";

					// Update button activity
					this->listView1->Items->Clear();
					if(forwarderState.controllerConnected)
					{
						this->listView1->Enabled = true;

						int currentRow = 0;

						// Buttons
						for(int i = 0; i <= 20; i++) {

							if((int)forwarderState.input[i] != (int)0 || (int)forwarderState.output[i] != (int)0)
							{
								this->listView1->Items->Insert(currentRow, buttonLabel[i]);

								// Prefix >= 0 with a plus sign
								if((int)forwarderState.input[i] >= (int)0)
								{
									this->listView1->Items[currentRow]->SubItems->Add("+" + Convert::ToString(forwarderState.input[i]));
								}
								else
								{
									this->listView1->Items[currentRow]->SubItems->Add(Convert::ToString(forwarderState.input[i]));
								}

								if((int)forwarderState.output[i] >= (int)0)
								{
									this->listView1->Items[currentRow]->SubItems->Add("+" + Convert::ToString(forwarderState.output[i]));
								}
								else
								{
									this->listView1->Items[currentRow]->SubItems->Add(Convert::ToString(forwarderState.output[i]));
								}

								currentRow++;
							}

						}

						// Rumble
						for(int i = 0; i <= 3; i++) {

							if((int)forwarderState.rumble_in[i] != (int)0 || (int)forwarderState.rumble_out[i] != (int)0)
							{
								this->listView1->Items->Insert(currentRow, rumbleLabel[i]);

								// Prefix with a plus sign
								this->listView1->Items[currentRow]->SubItems->Add("+" + Convert::ToString(forwarderState.rumble_in[i]));
								this->listView1->Items[currentRow]->SubItems->Add("+" + Convert::ToString(forwarderState.rumble_out[i]));

								currentRow++;
							}

						}
						
					}
					else
					{
						this->listView1->Enabled = false;
					}
				}

				isProcessingBackgroundWorkerReport = false;
			}
			
		}

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->notifyIcon = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->notifyContextMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->miToggleVisible = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->miExit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->linkLabel2 = (gcnew System::Windows::Forms::LinkLabel());
			this->Author = (gcnew System::Windows::Forms::Label());
			this->linkLabel1 = (gcnew System::Windows::Forms::LinkLabel());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->statusStrip1->SuspendLayout();
			this->notifyContextMenu->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->tabPage4->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->SuspendLayout();
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatusLabel1});
			this->statusStrip1->Location = System::Drawing::Point(0, 317);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(374, 22);
			this->statusStrip1->TabIndex = 2;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(180, 17);
			this->toolStripStatusLabel1->Text = L"Waiting for controller and device";
			// 
			// listView1
			// 
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader1, this->columnHeader2, 
				this->columnHeader3});
			this->listView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listView1->Enabled = false;
			this->listView1->Font = (gcnew System::Drawing::Font(L"Consolas", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->listView1->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->listView1->Location = System::Drawing::Point(3, 3);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(360, 289);
			this->listView1->TabIndex = 3;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader1
			// 
			this->columnHeader1->Text = L"Button";
			this->columnHeader1->Width = 180;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"Input";
			this->columnHeader2->Width = 50;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"Output";
			this->columnHeader3->Width = 50;
			// 
			// notifyIcon
			// 
			this->notifyIcon->ContextMenuStrip = this->notifyContextMenu;
			this->notifyIcon->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"notifyIcon.Icon")));
			this->notifyIcon->Text = L"CronusMAX XInput";
			this->notifyIcon->Visible = true;
			this->notifyIcon->DoubleClick += gcnew System::EventHandler(this, &Form1::notifyIcon_DoubleClick);
			// 
			// notifyContextMenu
			// 
			this->notifyContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->miToggleVisible, 
				this->miExit});
			this->notifyContextMenu->Name = L"miExit";
			this->notifyContextMenu->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->notifyContextMenu->Size = System::Drawing::Size(147, 48);
			// 
			// miToggleVisible
			// 
			this->miToggleVisible->Name = L"miToggleVisible";
			this->miToggleVisible->Size = System::Drawing::Size(146, 22);
			this->miToggleVisible->Text = L"Hide Window";
			this->miToggleVisible->Click += gcnew System::EventHandler(this, &Form1::miToggleVisible_Click);
			// 
			// miExit
			// 
			this->miExit->Name = L"miExit";
			this->miExit->Size = System::Drawing::Size(146, 22);
			this->miExit->Text = L"E&xit";
			this->miExit->Click += gcnew System::EventHandler(this, &Form1::miExit_Click);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker1_DoWork);
			this->backgroundWorker1->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Form1::backgroundWorker1_ProgressChanged);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(374, 317);
			this->tabControl1->TabIndex = 4;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->listView1);
			this->tabPage1->Location = System::Drawing::Point(4, 24);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(366, 295);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Device Monitor";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->button2);
			this->tabPage2->Controls->Add(this->label7);
			this->tabPage2->Controls->Add(this->groupBox3);
			this->tabPage2->Controls->Add(this->groupBox2);
			this->tabPage2->Controls->Add(this->groupBox1);
			this->tabPage2->Location = System::Drawing::Point(4, 24);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(366, 289);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Options";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radioButton4);
			this->groupBox1->Controls->Add(this->radioButton3);
			this->groupBox1->Controls->Add(this->radioButton2);
			this->groupBox1->Controls->Add(this->radioButton1);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(8, 6);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(350, 87);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Input Wrapper";
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(8, 44);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(334, 40);
			this->label1->TabIndex = 1;
			this->label1->Text = L"XInput wrappers are provided. Xbox One requires driver install. x360ce must be co" 
				L"nfigured separately.";
			// 
			// tabPage4
			// 
			this->tabPage4->Controls->Add(this->label6);
			this->tabPage4->Controls->Add(this->label5);
			this->tabPage4->Controls->Add(this->label4);
			this->tabPage4->Controls->Add(this->linkLabel2);
			this->tabPage4->Controls->Add(this->Author);
			this->tabPage4->Controls->Add(this->linkLabel1);
			this->tabPage4->Controls->Add(this->label3);
			this->tabPage4->Controls->Add(this->label2);
			this->tabPage4->Location = System::Drawing::Point(4, 24);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(366, 278);
			this->tabPage4->TabIndex = 3;
			this->tabPage4->Text = L"About";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// label6
			// 
			this->label6->Location = System::Drawing::Point(8, 123);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(350, 28);
			this->label6->TabIndex = 7;
			this->label6->Text = L"Misadri of ControllerMAX Forums";
			this->label6->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// label5
			// 
			this->label5->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(9, 209);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(350, 22);
			this->label5->TabIndex = 6;
			this->label5->Text = L"Source Code";
			this->label5->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// label4
			// 
			this->label4->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(6, 155);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(350, 22);
			this->label4->TabIndex = 5;
			this->label4->Text = L"Support";
			this->label4->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// linkLabel2
			// 
			this->linkLabel2->Location = System::Drawing::Point(9, 231);
			this->linkLabel2->Name = L"linkLabel2";
			this->linkLabel2->Size = System::Drawing::Size(349, 25);
			this->linkLabel2->TabIndex = 4;
			this->linkLabel2->TabStop = true;
			this->linkLabel2->Text = L"github.com/badgio/CronusMAX-XInput";
			this->linkLabel2->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->linkLabel2->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &Form1::linkLabel2_LinkClicked);
			// 
			// Author
			// 
			this->Author->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Author->Location = System::Drawing::Point(9, 101);
			this->Author->Name = L"Author";
			this->Author->Size = System::Drawing::Size(350, 22);
			this->Author->TabIndex = 3;
			this->Author->Text = L"Author";
			this->Author->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// linkLabel1
			// 
			this->linkLabel1->Location = System::Drawing::Point(9, 177);
			this->linkLabel1->Name = L"linkLabel1";
			this->linkLabel1->Size = System::Drawing::Size(349, 28);
			this->linkLabel1->TabIndex = 2;
			this->linkLabel1->TabStop = true;
			this->linkLabel1->Text = L"controllermax.com/forums";
			this->linkLabel1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->linkLabel1->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &Form1::linkLabel1_LinkClicked);
			// 
			// label3
			// 
			this->label3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(8, 26);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(350, 21);
			this->label3->TabIndex = 1;
			this->label3->Text = L"CronusMAX-XInput";
			this->label3->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(8, 54);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(350, 28);
			this->label2->TabIndex = 0;
			this->label2->Text = L"XInput plugin for GPP/CronusMAX device";
			this->label2->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(10, 22);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(112, 19);
			this->radioButton1->TabIndex = 2;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"None (Xbox 360)";
			this->radioButton1->UseVisualStyleBackColor = true;
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(132, 22);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(76, 19);
			this->radioButton2->TabIndex = 3;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"Xbox One";
			this->radioButton2->UseVisualStyleBackColor = true;
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(222, 22);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(44, 19);
			this->radioButton3->TabIndex = 4;
			this->radioButton3->TabStop = true;
			this->radioButton3->Text = L"PS3";
			this->radioButton3->UseVisualStyleBackColor = true;
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Location = System::Drawing::Point(281, 22);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(60, 19);
			this->radioButton4->TabIndex = 5;
			this->radioButton4->TabStop = true;
			this->radioButton4->Text = L"x360ce";
			this->radioButton4->UseVisualStyleBackColor = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->label8);
			this->groupBox2->Controls->Add(this->button1);
			this->groupBox2->Location = System::Drawing::Point(8, 99);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(350, 58);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"GPC scripting";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(10, 22);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(120, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Edit GPC script";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->checkBox1);
			this->groupBox3->Location = System::Drawing::Point(8, 163);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(349, 53);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Configurations";
			// 
			// label7
			// 
			this->label7->ForeColor = System::Drawing::Color::DarkRed;
			this->label7->Location = System::Drawing::Point(10, 228);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(350, 22);
			this->label7->TabIndex = 3;
			this->label7->Text = L"Changes will not take effect until you restart the application.";
			this->label7->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// label8
			// 
			this->label8->Location = System::Drawing::Point(136, 19);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(206, 30);
			this->label8->TabIndex = 2;
			this->label8->Text = L"Scripting via a C interpreter.  Not all GPC language features are supported.";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(11, 23);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(286, 19);
			this->checkBox1->TabIndex = 0;
			this->checkBox1->Text = L"Passthrough input from authentication controller";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(101, 252);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(164, 23);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Save Options && Restart XInput";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(374, 339);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->statusStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"XInput";
			this->Resize += gcnew System::EventHandler(this, &Form1::Form1_Resize);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->notifyContextMenu->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->tabPage4->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

		System::Void notifyIcon_DoubleClick(System::Object^ sender, System::EventArgs^  e)
		{
			this->WindowState = FormWindowState::Minimized;
			this->Visible = true;
			this->WindowState = FormWindowState::Normal;
		}

		System::Void miToggleVisible_Click(System::Object^ sender, System::EventArgs^  e)
		{
			if(this->WindowState == FormWindowState::Minimized)
			{
				this->Visible = true;
				this->WindowState = FormWindowState::Normal;
			}
			else if(this->WindowState == FormWindowState::Normal)
			{
				this->WindowState = FormWindowState::Minimized;
			}
		}

		System::Void miExit_Click(System::Object^ sender, System::EventArgs^  e)
		{
			Application::Exit();
		}

		System::Void Form1_Resize(System::Object^  sender, System::EventArgs^  e) {
			if(this->WindowState == FormWindowState::Minimized)
			{
				this->Visible = false;
				this->miToggleVisible->Text = L"Restore Window";
			}
			else if(this->WindowState == FormWindowState::Normal)
			{
				this->miToggleVisible->Text = L"Hide Window";
			}
		}

#pragma endregion

private: System::Void linkLabel1_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
			 Process::Start( "http://controllermax.com/forums/forumdisplay.php?f=276" );
		 }
private: System::Void linkLabel2_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
			 Process::Start( "https://github.com/badgio/CronusMAX-XInput" );
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

