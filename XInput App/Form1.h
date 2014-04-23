#pragma once

namespace CronusMAX_XInput {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
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
			this->statusStrip1->SuspendLayout();
			this->notifyContextMenu->SuspendLayout();
			this->SuspendLayout();
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatusLabel1});
			this->statusStrip1->Location = System::Drawing::Point(0, 193);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(301, 22);
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
			this->listView1->Location = System::Drawing::Point(0, 0);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(301, 193);
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
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(301, 215);
			this->Controls->Add(this->listView1);
			this->Controls->Add(this->statusStrip1);
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

	};
}

