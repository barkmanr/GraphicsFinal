#pragma once
#include <iomanip>
#include "GameController.h"
#include "Mesh.h"

namespace Tag {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ToolWindow : public System::Windows::Forms::Form
	{
	public:
		static bool _light;
		static bool _transform;
		static bool _water;
		static bool _space;

		static int SpecStrength;
		static int R;
		static int G;
		static int B;

		static bool _translate;
		static bool _rotate;
		static bool _scale;

		static int F;
		static int A;
		static bool _wireframe;
		static bool _blue;

	private: System::Windows::Forms::TrackBar^ TrackB;
	private: System::Windows::Forms::TrackBar^ TrackG;
	private: System::Windows::Forms::TrackBar^ TrackR;
	private: System::Windows::Forms::TrackBar^ SpecularStrength;
	private: System::Windows::Forms::Label^ LB;
	private: System::Windows::Forms::Label^ LG;
	private: System::Windows::Forms::Label^ LR;
	private: System::Windows::Forms::Label^ LSpec;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ ResetLightPosition;
	private: System::Windows::Forms::RadioButton^ Transform;
	private: System::Windows::Forms::RadioButton^ WaterScene;
	private: System::Windows::Forms::RadioButton^ SpaceScene;
	private: System::Windows::Forms::Button^ ResetTransform;
	private: System::Windows::Forms::CheckBox^ Translate;
	private: System::Windows::Forms::CheckBox^ Rotate;
	private: System::Windows::Forms::CheckBox^ Scale;
	private: System::Windows::Forms::CheckBox^ WireFrame;
	private: System::Windows::Forms::CheckBox^ BlueTint;
	private: System::Windows::Forms::TrackBar^ Frequency;
	private: System::Windows::Forms::TrackBar^ Amplitude;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ LFreq;
	private: System::Windows::Forms::Label^ LAmp;
	private: System::Windows::Forms::RadioButton^ MoveLight;

	public:
		ToolWindow(void)
		{
			InitializeComponent();
			_light = MoveLight->Checked;
			_transform = Transform->Checked;
			_water = WaterScene->Checked;
			_space = SpaceScene->Checked;

			SpecStrength = SpecularStrength->Value;
			R = TrackR->Value;
			G = TrackG->Value;
			B = TrackB->Value;

			LSpec->Text = SpecStrength.ToString();
			LR->Text = ((float)R / 100.0f).ToString();
			LG->Text = ((float)G / 100.0f).ToString();
			LB->Text = ((float)B / 100.0f).ToString();

			_translate = Translate->Checked;
			_rotate = Rotate->Checked;
			_scale = Scale->Checked;

			F = Frequency->Value;
			A = Amplitude->Value;
			LFreq->Text = ((float)F / 100.0f).ToString();
			LAmp->Text = ((float)A / 100.0f).ToString();
			_wireframe = WireFrame->Checked;
			_blue = BlueTint->Checked;
		}

	protected:
		~ToolWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->TrackB = (gcnew System::Windows::Forms::TrackBar());
			this->TrackG = (gcnew System::Windows::Forms::TrackBar());
			this->TrackR = (gcnew System::Windows::Forms::TrackBar());
			this->SpecularStrength = (gcnew System::Windows::Forms::TrackBar());
			this->LB = (gcnew System::Windows::Forms::Label());
			this->LG = (gcnew System::Windows::Forms::Label());
			this->LR = (gcnew System::Windows::Forms::Label());
			this->LSpec = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ResetLightPosition = (gcnew System::Windows::Forms::Button());
			this->MoveLight = (gcnew System::Windows::Forms::RadioButton());
			this->Transform = (gcnew System::Windows::Forms::RadioButton());
			this->WaterScene = (gcnew System::Windows::Forms::RadioButton());
			this->SpaceScene = (gcnew System::Windows::Forms::RadioButton());
			this->ResetTransform = (gcnew System::Windows::Forms::Button());
			this->Translate = (gcnew System::Windows::Forms::CheckBox());
			this->Rotate = (gcnew System::Windows::Forms::CheckBox());
			this->Scale = (gcnew System::Windows::Forms::CheckBox());
			this->WireFrame = (gcnew System::Windows::Forms::CheckBox());
			this->BlueTint = (gcnew System::Windows::Forms::CheckBox());
			this->Frequency = (gcnew System::Windows::Forms::TrackBar());
			this->Amplitude = (gcnew System::Windows::Forms::TrackBar());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->LFreq = (gcnew System::Windows::Forms::Label());
			this->LAmp = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackB))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackG))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackR))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SpecularStrength))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Frequency))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Amplitude))->BeginInit();
			this->SuspendLayout();
			// 
			// TrackB
			// 
			this->TrackB->BackColor = System::Drawing::SystemColors::Control;
			this->TrackB->Location = System::Drawing::Point(159, 269);
			this->TrackB->Maximum = 300;
			this->TrackB->Name = L"TrackB";
			this->TrackB->Size = System::Drawing::Size(255, 45);
			this->TrackB->TabIndex = 30;
			this->TrackB->Value = 100;
			this->TrackB->Scroll += gcnew System::EventHandler(this, &ToolWindow::TrackB_Scroll);
			// 
			// TrackG
			// 
			this->TrackG->BackColor = System::Drawing::SystemColors::Control;
			this->TrackG->Location = System::Drawing::Point(159, 208);
			this->TrackG->Maximum = 300;
			this->TrackG->Name = L"TrackG";
			this->TrackG->Size = System::Drawing::Size(255, 45);
			this->TrackG->TabIndex = 29;
			this->TrackG->Value = 100;
			this->TrackG->Scroll += gcnew System::EventHandler(this, &ToolWindow::TrackG_Scroll);
			// 
			// TrackR
			// 
			this->TrackR->BackColor = System::Drawing::SystemColors::Control;
			this->TrackR->Location = System::Drawing::Point(159, 149);
			this->TrackR->Maximum = 300;
			this->TrackR->Name = L"TrackR";
			this->TrackR->Size = System::Drawing::Size(255, 45);
			this->TrackR->TabIndex = 28;
			this->TrackR->Value = 100;
			this->TrackR->Scroll += gcnew System::EventHandler(this, &ToolWindow::TrackR_Scroll);
			// 
			// SpecularStrength
			// 
			this->SpecularStrength->BackColor = System::Drawing::SystemColors::Control;
			this->SpecularStrength->Location = System::Drawing::Point(159, 80);
			this->SpecularStrength->Maximum = 128;
			this->SpecularStrength->Minimum = 1;
			this->SpecularStrength->Name = L"SpecularStrength";
			this->SpecularStrength->Size = System::Drawing::Size(255, 45);
			this->SpecularStrength->TabIndex = 27;
			this->SpecularStrength->Value = 4;
			this->SpecularStrength->Scroll += gcnew System::EventHandler(this, &ToolWindow::SpecularStrength_Scroll);
			// 
			// LB
			// 
			this->LB->AutoSize = true;
			this->LB->Location = System::Drawing::Point(430, 269);
			this->LB->Name = L"LB";
			this->LB->Size = System::Drawing::Size(35, 13);
			this->LB->TabIndex = 26;
			this->LB->Text = L"label9";
			// 
			// LG
			// 
			this->LG->AutoSize = true;
			this->LG->Location = System::Drawing::Point(430, 208);
			this->LG->Name = L"LG";
			this->LG->Size = System::Drawing::Size(35, 13);
			this->LG->TabIndex = 25;
			this->LG->Text = L"label8";
			// 
			// LR
			// 
			this->LR->AutoSize = true;
			this->LR->Location = System::Drawing::Point(430, 149);
			this->LR->Name = L"LR";
			this->LR->Size = System::Drawing::Size(35, 13);
			this->LR->TabIndex = 24;
			this->LR->Text = L"label7";
			// 
			// LSpec
			// 
			this->LSpec->AutoSize = true;
			this->LSpec->Location = System::Drawing::Point(430, 80);
			this->LSpec->Name = L"LSpec";
			this->LSpec->Size = System::Drawing::Size(35, 13);
			this->LSpec->TabIndex = 23;
			this->LSpec->Text = L"label6";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(130, 269);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(14, 13);
			this->label5->TabIndex = 22;
			this->label5->Text = L"B";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(129, 208);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(15, 13);
			this->label4->TabIndex = 21;
			this->label4->Text = L"G";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(129, 149);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(15, 13);
			this->label3->TabIndex = 20;
			this->label3->Text = L"R";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(61, 122);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(73, 13);
			this->label2->TabIndex = 19;
			this->label2->Text = L"SpecularColor";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(61, 80);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(92, 13);
			this->label1->TabIndex = 18;
			this->label1->Text = L"Specular Strength";
			// 
			// ResetLightPosition
			// 
			this->ResetLightPosition->BackColor = System::Drawing::SystemColors::Control;
			this->ResetLightPosition->Location = System::Drawing::Point(38, 41);
			this->ResetLightPosition->Name = L"ResetLightPosition";
			this->ResetLightPosition->Size = System::Drawing::Size(148, 23);
			this->ResetLightPosition->TabIndex = 17;
			this->ResetLightPosition->Text = L"Reset Light Position";
			this->ResetLightPosition->UseVisualStyleBackColor = false;
			this->ResetLightPosition->Click += gcnew System::EventHandler(this, &ToolWindow::ResetLightPosition_Click);
			// 
			// MoveLight
			// 
			this->MoveLight->AutoSize = true;
			this->MoveLight->Checked = true;
			this->MoveLight->Location = System::Drawing::Point(27, 18);
			this->MoveLight->Name = L"MoveLight";
			this->MoveLight->Size = System::Drawing::Size(78, 17);
			this->MoveLight->TabIndex = 0;
			this->MoveLight->TabStop = true;
			this->MoveLight->Text = L"Move Light";
			this->MoveLight->UseVisualStyleBackColor = true;
			this->MoveLight->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::MoveLight_CheckedChanged);
			// 
			// Transform
			// 
			this->Transform->AutoSize = true;
			this->Transform->Location = System::Drawing::Point(33, 347);
			this->Transform->Name = L"Transform";
			this->Transform->Size = System::Drawing::Size(72, 17);
			this->Transform->TabIndex = 0;
			this->Transform->TabStop = true;
			this->Transform->Text = L"Transform";
			this->Transform->UseVisualStyleBackColor = true;
			this->Transform->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::Transform_CheckedChanged);
			// 
			// WaterScene
			// 
			this->WaterScene->AutoSize = true;
			this->WaterScene->Location = System::Drawing::Point(33, 460);
			this->WaterScene->Name = L"WaterScene";
			this->WaterScene->Size = System::Drawing::Size(88, 17);
			this->WaterScene->TabIndex = 0;
			this->WaterScene->TabStop = true;
			this->WaterScene->Text = L"Water Scene";
			this->WaterScene->UseVisualStyleBackColor = true;
			this->WaterScene->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::WaterScene_CheckedChanged);
			// 
			// SpaceScene
			// 
			this->SpaceScene->AutoSize = true;
			this->SpaceScene->Location = System::Drawing::Point(33, 625);
			this->SpaceScene->Name = L"SpaceScene";
			this->SpaceScene->Size = System::Drawing::Size(90, 17);
			this->SpaceScene->TabIndex = 0;
			this->SpaceScene->TabStop = true;
			this->SpaceScene->Text = L"Space Scene";
			this->SpaceScene->UseVisualStyleBackColor = true;
			this->SpaceScene->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::SpaceScene_CheckedChanged);
			// 
			// ResetTransform
			// 
			this->ResetTransform->Location = System::Drawing::Point(52, 370);
			this->ResetTransform->Name = L"ResetTransform";
			this->ResetTransform->Size = System::Drawing::Size(122, 23);
			this->ResetTransform->TabIndex = 31;
			this->ResetTransform->Text = L"Reset Transform";
			this->ResetTransform->UseVisualStyleBackColor = true;
			this->ResetTransform->Click += gcnew System::EventHandler(this, &ToolWindow::ResetTransform_Click);
			// 
			// Translate
			// 
			this->Translate->AutoSize = true;
			this->Translate->Location = System::Drawing::Point(93, 412);
			this->Translate->Name = L"Translate";
			this->Translate->Size = System::Drawing::Size(70, 17);
			this->Translate->TabIndex = 32;
			this->Translate->Text = L"Translate";
			this->Translate->UseVisualStyleBackColor = true;
			this->Translate->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::Translate_CheckedChanged);
			// 
			// Rotate
			// 
			this->Rotate->AutoSize = true;
			this->Rotate->Location = System::Drawing::Point(213, 412);
			this->Rotate->Name = L"Rotate";
			this->Rotate->Size = System::Drawing::Size(58, 17);
			this->Rotate->TabIndex = 33;
			this->Rotate->Text = L"Rotate";
			this->Rotate->UseVisualStyleBackColor = true;
			this->Rotate->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::Rotate_CheckedChanged);
			// 
			// Scale
			// 
			this->Scale->AutoSize = true;
			this->Scale->Location = System::Drawing::Point(327, 412);
			this->Scale->Name = L"Scale";
			this->Scale->Size = System::Drawing::Size(53, 17);
			this->Scale->TabIndex = 34;
			this->Scale->Text = L"Scale";
			this->Scale->UseVisualStyleBackColor = true;
			this->Scale->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::Scale_CheckedChanged);
			// 
			// WireFrame
			// 
			this->WireFrame->AutoSize = true;
			this->WireFrame->Location = System::Drawing::Point(81, 583);
			this->WireFrame->Name = L"WireFrame";
			this->WireFrame->Size = System::Drawing::Size(112, 17);
			this->WireFrame->TabIndex = 35;
			this->WireFrame->Text = L"Wireframe Render";
			this->WireFrame->UseVisualStyleBackColor = true;
			this->WireFrame->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::WireFrame_CheckedChanged);
			// 
			// BlueTint
			// 
			this->BlueTint->AutoSize = true;
			this->BlueTint->Location = System::Drawing::Point(203, 583);
			this->BlueTint->Name = L"BlueTint";
			this->BlueTint->Size = System::Drawing::Size(68, 17);
			this->BlueTint->TabIndex = 36;
			this->BlueTint->Text = L"Tint Blue";
			this->BlueTint->UseVisualStyleBackColor = true;
			this->BlueTint->CheckedChanged += gcnew System::EventHandler(this, &ToolWindow::BlueTint_CheckedChanged);
			// 
			// Frequency
			// 
			this->Frequency->Location = System::Drawing::Point(93, 483);
			this->Frequency->Maximum = 400;
			this->Frequency->Name = L"Frequency";
			this->Frequency->Size = System::Drawing::Size(216, 45);
			this->Frequency->TabIndex = 37;
			this->Frequency->Value = 100;
			this->Frequency->Scroll += gcnew System::EventHandler(this, &ToolWindow::Frequency_Scroll);
			// 
			// Amplitude
			// 
			this->Amplitude->Location = System::Drawing::Point(93, 535);
			this->Amplitude->Maximum = 100;
			this->Amplitude->Name = L"Amplitude";
			this->Amplitude->Size = System::Drawing::Size(216, 45);
			this->Amplitude->TabIndex = 38;
			this->Amplitude->Scroll += gcnew System::EventHandler(this, &ToolWindow::Amplitude_Scroll);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(38, 484);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(57, 13);
			this->label6->TabIndex = 39;
			this->label6->Text = L"Frequency";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(41, 535);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(53, 13);
			this->label7->TabIndex = 40;
			this->label7->Text = L"Amplitude";
			// 
			// LFreq
			// 
			this->LFreq->AutoSize = true;
			this->LFreq->Location = System::Drawing::Point(344, 484);
			this->LFreq->Name = L"LFreq";
			this->LFreq->Size = System::Drawing::Size(35, 13);
			this->LFreq->TabIndex = 41;
			this->LFreq->Text = L"label8";
			// 
			// LAmp
			// 
			this->LAmp->AutoSize = true;
			this->LAmp->Location = System::Drawing::Point(347, 534);
			this->LAmp->Name = L"LAmp";
			this->LAmp->Size = System::Drawing::Size(35, 13);
			this->LAmp->TabIndex = 42;
			this->LAmp->Text = L"label9";
			// 
			// ToolWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(523, 680);
			this->Controls->Add(this->LAmp);
			this->Controls->Add(this->LFreq);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->Amplitude);
			this->Controls->Add(this->Frequency);
			this->Controls->Add(this->BlueTint);
			this->Controls->Add(this->WireFrame);
			this->Controls->Add(this->Scale);
			this->Controls->Add(this->Rotate);
			this->Controls->Add(this->Translate);
			this->Controls->Add(this->ResetTransform);
			this->Controls->Add(this->SpaceScene);
			this->Controls->Add(this->WaterScene);
			this->Controls->Add(this->Transform);
			this->Controls->Add(this->TrackB);
			this->Controls->Add(this->TrackG);
			this->Controls->Add(this->TrackR);
			this->Controls->Add(this->SpecularStrength);
			this->Controls->Add(this->LB);
			this->Controls->Add(this->LG);
			this->Controls->Add(this->LR);
			this->Controls->Add(this->LSpec);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ResetLightPosition);
			this->Controls->Add(this->MoveLight);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"ToolWindow";
			this->Text = L"ToolWindow";
			this->TopMost = true;
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackB))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackG))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->TrackR))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->SpecularStrength))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Frequency))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Amplitude))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		private: System::Void MoveLight_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			_light = MoveLight->Checked;
			_transform = Transform->Checked;
			_water = WaterScene->Checked;
			_space = SpaceScene->Checked;
		}
		private: System::Void Transform_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			_light = MoveLight->Checked;
			_transform = Transform->Checked;
			_water = WaterScene->Checked;
			_space = SpaceScene->Checked;
		}
		private: System::Void WaterScene_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			_light = MoveLight->Checked;
			_transform = Transform->Checked;
			_water = WaterScene->Checked;
			_space = SpaceScene->Checked;
		}
		private: System::Void SpaceScene_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
		{
			_light = MoveLight->Checked;
			_transform = Transform->Checked;
			_water = WaterScene->Checked;
			_space = SpaceScene->Checked;
		}

		private: System::Void ResetLightPosition_Click(System::Object^ sender, System::EventArgs^ e) 
		{
			for (Mesh* light : GameController::Instance().GetLights())
			{
				light->SetPosition({ 0.0f,0.0f,2.0f });
			}
		}

		private: System::Void SpecularStrength_Scroll(System::Object^ sender, System::EventArgs^ e) 
		{
			SpecStrength = SpecularStrength->Value;
			LSpec->Text = SpecStrength.ToString();
			GameController::Instance().GetShip()->SetSpec(SpecStrength);
		}
		private: System::Void TrackR_Scroll(System::Object^ sender, System::EventArgs^ e) 
		{
			R = TrackR->Value;
			LR->Text = ((float)R / 100.0f).ToString();
			glm::vec3 color = glm::vec3{ R,G,B };
			color *= 0.01;
			GameController::Instance().GetLights()[0]->SetSpecColor(color);
		}
		private: System::Void TrackG_Scroll(System::Object^ sender, System::EventArgs^ e) 
		{
			G = TrackG->Value;
			LG->Text = ((float)G / 100.0f).ToString();
			glm::vec3 color = { R,G,B };
			color *= 0.01;
			GameController::Instance().GetLights()[0]->SetSpecColor(color);
		}
		private: System::Void TrackB_Scroll(System::Object^ sender, System::EventArgs^ e) 
		{
			B = TrackB->Value;
			LB->Text = ((float)B / 100.0f).ToString();
			glm::vec3 color = { R,G,B };
			color *= 0.01;
			GameController::Instance().GetLights()[0]->SetSpecColor(color);
		}

		private: System::Void ResetTransform_Click(System::Object^ sender, System::EventArgs^ e)
			{
			GameController::Instance().SetShip();
			}

private: System::Void Translate_CheckedChanged(System::Object^ sender, System::EventArgs^ e) 
{
	_translate = Translate->Checked;
}
private: System::Void Rotate_CheckedChanged(System::Object^ sender, System::EventArgs^ e) 
{
	_rotate = Rotate->Checked;
}
private: System::Void Scale_CheckedChanged(System::Object^ sender, System::EventArgs^ e) 
{
	_scale = Scale->Checked;
}
private: System::Void WireFrame_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	_wireframe = WireFrame->Checked;
}
private: System::Void BlueTint_CheckedChanged(System::Object^ sender, System::EventArgs^ e) 
{
	_blue = BlueTint->Checked;
}
private: System::Void Frequency_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	F = Frequency->Value;
	LFreq->Text = ((float)F/100.0f).ToString();
}
private: System::Void Amplitude_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	A = Amplitude->Value;
	LAmp->Text = ((float)A/100.0f).ToString();
}
};
}
