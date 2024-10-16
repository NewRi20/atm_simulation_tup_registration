#include "MainFrame.h"
#include "classes.h"
#include <windows.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>
#include <wx/regex.h>


using namespace std;

MainFrame::MainFrame(const wxString& title) 
    : wxFrame(nullptr, wxID_ANY, title), 
    adminAccount("00000", "admin", "123456", "09-28-2004", "+63 909-320-5093", 1000000.0),
    bank(adminAccount) 
{  
    
    /*wxMessageBox(wxString::Format("Admin Balance: %f", adminAccount.balance), "Balance Inquiry", wxOK | wxICON_INFORMATION);
    wxMessageBox(wxString::Format("Current Account Balance: %f", bank.currentAccount.balance), "Balance Inquiry", wxOK | wxICON_INFORMATION);*/
    bank.retrieveAllAccounts();
    isLoggedIn = false;
    CreateControls();
    SetupSizers();
    BindEventHandlers();
}


void MainFrame::CreateControls()
{
    wxFont headlineFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    wxFont subtextFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    wxFont mainFont(wxFontInfo(wxSize(0, 20)));
    wxFont customTitleFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    wxFont bankNameFont(30, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD);

    panel = new wxPanel(this);
    panel->SetFont(mainFont);

    timer = new wxTimer(this, wxID_ANY);
    timer->Start(2000);

    //Bank name
    bankName = new wxStaticText(panel, wxID_ANY, "TechnoBank");
    bankName->SetFont(bankNameFont);
    bankName->SetForegroundColour(wxColour(235, 250, 219)); //192, 192, 192

    //InsertCard text
    wxFont customFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_SLANT, wxFONTWEIGHT_BOLD);
    insertCardText = new wxStaticText(panel, wxID_ANY, "Please insert card...", wxDefaultPosition, wxDefaultSize);
    insertCardText->SetFont(customFont);
    insertCardText->SetForegroundColour(wxColor(235, 250, 219));//192, 192, 192

    // Enter PIN Code Field
    enterPincodeText = new wxStaticText(panel, wxID_ANY, "Enter PIN Code");
    enterPincodeText->SetFont(customTitleFont);
    enterPincodeText->SetForegroundColour(wxColor(235, 250, 219));//215, 215, 215
    pincodeInputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    enterPincodeButton = new wxButton(panel, wxID_ANY, "Enter PIN Code", wxDefaultPosition, wxDefaultSize);

    // Registration Controlsz
    registrationText = new wxStaticText(panel, wxID_ANY, "Register for an Account", wxPoint(200, 100));
    registrationText->SetFont(customTitleFont);
    registrationText->SetForegroundColour(wxColor(235, 250, 219));//215, 215, 215


    //Text for inputFields
    wxFont InputheadlineFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    accountNameInputField = new wxTextCtrl(panel, wxID_ANY, "Name: ", wxDefaultPosition, wxDefaultSize);
    accountNameInputField->SetFont(InputheadlineFont);
    accountNameInputField->SetForegroundColour(wxColor(118, 127, 125));//167, 172, 180
    birthdayInputField = new wxTextCtrl(panel, wxID_ANY, "Birthday (MM-DD-YYYY): ", wxDefaultPosition, wxDefaultSize);
    birthdayInputField->SetFont(InputheadlineFont);
    birthdayInputField->SetForegroundColour(wxColor(118, 127, 125));//167, 172, 180
    contactNumberInputField = new wxTextCtrl(panel, wxID_ANY, "Contact Number(00000000000) : ", wxDefaultPosition, wxDefaultSize);
    contactNumberInputField->SetFont(InputheadlineFont);
    contactNumberInputField->SetForegroundColour(wxColor(118, 127, 125));//167, 172, 180
    pinCodeInputField = new wxTextCtrl(panel, wxID_ANY, "PIN Code (000000): ", wxDefaultPosition, wxDefaultSize);
    pinCodeInputField->SetFont(InputheadlineFont);
    pinCodeInputField->SetForegroundColour(wxColor(118, 127, 125));//167, 172, 180

    registerButton = new wxButton(panel, wxID_ANY, "Register");

    // Transaction Controls
    transactionText = new wxStaticText(panel, wxID_ANY, "Choose a Transaction");
    transactionText->SetFont(customTitleFont);
    transactionText->SetForegroundColour(*wxWHITE);
    balanceInquiryButton = new wxButton(panel, wxID_ANY, "Balance Inquiry");
    withdrawButton = new wxButton(panel, wxID_ANY, "Withdraw");
    depositButton = new wxButton(panel, wxID_ANY, "Deposit");
    fundTransferButton = new wxButton(panel, wxID_ANY, "Fund Transfer");
    changePinButton = new wxButton(panel, wxID_ANY, "Change PIN Code");

    //InputFieldSizes
    int maxInputWidth = 650; 

    accountNameInputField->SetMaxSize(wxSize(maxInputWidth, -1)); 
    birthdayInputField->SetMaxSize(wxSize(maxInputWidth, -1));
    contactNumberInputField->SetMaxSize(wxSize(maxInputWidth, -1));
    pinCodeInputField->SetMaxSize(wxSize(maxInputWidth, -1));

    accountNameInputField->SetMinSize(wxSize(maxInputWidth, -1));
    birthdayInputField->SetMinSize(wxSize(maxInputWidth, -1));
    contactNumberInputField->SetMinSize(wxSize(maxInputWidth, -1));
    pinCodeInputField->SetMinSize(wxSize(maxInputWidth, -1));

    //                  INSIDE TRANS
    // Balance Inquiry Controls
    //BalanceText = new wxStaticText(panel, wxID_ANY, "Current Balance");

    // balance

    BalanceText = new wxStaticText(panel, wxID_ANY, "Current Balance");
    BalanceText->SetFont(subtextFont);
    BalanceText->SetForegroundColour(wxColor(16, 93, 94));
    ConfirmBalanceInquiryButton = new wxButton(panel, wxID_ANY, "Confirm", wxDefaultPosition, wxDefaultSize);

    //withraw Controls
    WithrawText = new wxStaticText(panel, wxID_ANY, "Withraw Transaction", wxDefaultPosition, wxDefaultSize);
    WithrawText->SetFont(subtextFont);
    WithrawText->SetForegroundColour(wxColor(16, 93, 94));
    WithrawInputField = new wxTextCtrl(panel, wxID_ANY, "Withdraw amount: ", wxDefaultPosition, wxDefaultSize);
    ConfirmWithrawButton = new wxButton(panel, wxID_ANY, "Confirm", wxDefaultPosition, wxDefaultSize);

    //  DEPOSIT CONTROLS
    DepositText = new wxStaticText(panel, wxID_ANY, "Deposit Transaction", wxDefaultPosition, wxDefaultSize);
    DepositText->SetFont(subtextFont);
    DepositText->SetForegroundColour(wxColor(16, 93, 94));
    DepositInputField = new wxTextCtrl(panel, wxID_ANY, "Deposit amount: ", wxDefaultPosition, wxDefaultSize);
    ConfirmDepositButton = new wxButton(panel, wxID_ANY, "Confirm", wxDefaultPosition, wxDefaultSize);

    //  FUND TRANSFER CONTROLS
    FundTransterText = new wxStaticText(panel, wxID_ANY, "Fund Transfer", wxDefaultPosition, wxDefaultSize);
    FundTransterText->SetFont(subtextFont);
    FundTransterText->SetForegroundColour(wxColor(16, 93, 94));
    receiverAccountInputField = new wxTextCtrl(panel, wxID_ANY, "Recipient Account Number: ", wxDefaultPosition, wxDefaultSize);
    fundTransferAmountInputField = new wxTextCtrl(panel, wxID_ANY, "Transfer amount: ", wxDefaultPosition, wxDefaultSize);
    ConfirmFundTransferButton = new wxButton(panel, wxID_ANY, "Confirm Transfer", wxDefaultPosition, wxDefaultSize);

    //  CHANGE PIN CODE CONTROLS
    ChangePincodeText = new wxStaticText(panel, wxID_ANY, "Change PinCode", wxDefaultPosition, wxDefaultSize);
    ChangePincodeText->SetFont(subtextFont);
    ChangePincodeText->SetForegroundColour(wxColor(16, 93, 94));
    oldPincodeInputField = new wxTextCtrl(panel, wxID_ANY, "Old PIN Code: ", wxDefaultPosition, wxDefaultSize);
    newPincodeInputField = new wxTextCtrl(panel, wxID_ANY, "New PIN Code: ", wxDefaultPosition, wxDefaultSize);
    ConfirmChangePincodeButton = new wxButton(panel, wxID_ANY, "Confirm Changes", wxPoint(0, 22), wxDefaultSize);



    ShowInsertCardText(true);
    ShowInsertCardText(true);
    ShowEnterPincode(false);
    ShowRegistrationControls(false);
    ShowTransactionControls(false);
    ShowBalanceInquiryControls(false);
    ShowWithrawTransactionControls(false);
    ShowDepositTransactionControls(false);
    ShowFundTransferTransactionControls(false);
    ShowChangePincodeTransactionControls(false);


    statusBar = CreateStatusBar();
    statusBar->SetDoubleBuffered(true);
}

void MainFrame::SetupSizers()
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    panel->SetBackgroundColour(wxColor(16, 93, 94)); //60, 63, 69

    //Gray panel, white panel background yung parent
    wxPanel* grayPanel = new wxPanel(panel, wxID_ANY);
    grayPanel->SetBackgroundColour(wxColor(16, 93, 94)); //60, 63, 69
    grayPanel->SetMaxSize(wxSize(1300, 1300));

    //Creating gray panel sizer. Set to gray panel
    wxBoxSizer* grayPanelSizer = new wxBoxSizer(wxVERTICAL);
    grayPanel->SetSizer(grayPanelSizer);

    //Small box panel, top left (para mas lalo macenter yung atm screen)
    wxPanel* smallBoxPanel = new wxPanel(grayPanel, wxID_ANY);
    smallBoxPanel->SetBackgroundColour(wxColour(16, 93, 94)); //60, 63, 69  //another possible wxColour(100, 100, 100)
    smallBoxPanel->SetMinSize(wxSize(100, 100));

    wxBoxSizer* smallBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    smallBoxPanel->SetSizer(smallBoxSizer);

    //Add to gray panel sizer
    grayPanelSizer->Add(smallBoxPanel, wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 5));

    //Black border panel, border ng atm screen
    wxPanel* blackBorderPanel = new wxPanel(grayPanel, wxID_ANY);
    blackBorderPanel->SetBackgroundColour(wxColour(42, 61, 51)); //56, 59, 65
    blackBorderPanel->SetMaxSize(wxSize(1410, 810));
    blackBorderPanel->SetMinSize(wxSize(710, 410));

    //Sizer for black border panel
    wxBoxSizer* blackBorderSizer = new wxBoxSizer(wxVERTICAL);
    blackBorderPanel->SetSizer(blackBorderSizer);

    //White box sizer panel with blackBorderPanel as its parent (ito yung atm screen)
    wxPanel* whiteBoxPanel = new wxPanel(blackBorderPanel, wxID_ANY);
    whiteBoxPanel->SetBackgroundColour(wxColor(179, 237, 169)); //60, 70, 92
    whiteBoxPanel->SetMaxSize(wxSize(1400, 800));
    whiteBoxPanel->SetMinSize(wxSize(700, 400)); // <-- optional, dapat iadjust lahat if ever ibahin ung MinSize

    //White box panel Sizer
    wxBoxSizer* whiteBoxPanelSizer = new wxBoxSizer(wxVERTICAL);
    whiteBoxPanel->SetSizer(whiteBoxPanelSizer);

    //Reparent the controls to the whiteBoxPanel(eto nagpatulong ako gpt, di ko alam na ay reparent pala)
    bankName->Reparent(grayPanel);
    enterPincodeText->Reparent(whiteBoxPanel);
    pincodeInputField->Reparent(whiteBoxPanel);
    enterPincodeButton->Reparent(whiteBoxPanel);
    registrationText->Reparent(whiteBoxPanel);
    accountNameInputField->Reparent(whiteBoxPanel);
    birthdayInputField->Reparent(whiteBoxPanel);
    contactNumberInputField->Reparent(whiteBoxPanel);
    pinCodeInputField->Reparent(whiteBoxPanel);
    registerButton->Reparent(whiteBoxPanel);
    transactionText->Reparent(whiteBoxPanel);
    balanceInquiryButton->Reparent(whiteBoxPanel);
    BalanceText->Reparent(whiteBoxPanel);
    ConfirmBalanceInquiryButton->Reparent(whiteBoxPanel);
    WithrawText->Reparent(whiteBoxPanel);
    WithrawInputField->Reparent(whiteBoxPanel);
    ConfirmWithrawButton->Reparent(whiteBoxPanel);
    withdrawButton->Reparent(whiteBoxPanel);
    depositButton->Reparent(whiteBoxPanel);
    DepositText->Reparent(whiteBoxPanel);
    DepositInputField->Reparent(whiteBoxPanel);
    ConfirmDepositButton->Reparent(whiteBoxPanel);
    fundTransferButton->Reparent(whiteBoxPanel);
    FundTransterText->Reparent(whiteBoxPanel);
    receiverAccountInputField->Reparent(whiteBoxPanel);
    fundTransferAmountInputField->Reparent(whiteBoxPanel);
    ConfirmFundTransferButton->Reparent(whiteBoxPanel);
    changePinButton->Reparent(whiteBoxPanel);
    ChangePincodeText->Reparent(whiteBoxPanel);
    oldPincodeInputField->Reparent(whiteBoxPanel);
    newPincodeInputField->Reparent(whiteBoxPanel);
    ConfirmChangePincodeButton->Reparent(whiteBoxPanel);
    insertCardText->Reparent(whiteBoxPanel);

    //Bank Name
    int BankxOffset = 350; //Right (+) or left (-)
    int BankyOffset = 50; //Down (+) or up (-)
    bankName->SetPosition(wxPoint((whiteBoxPanel->GetSize().GetWidth() - insertCardText->GetSize().GetWidth()) / 2 + BankxOffset,
        (whiteBoxPanel->GetSize().GetHeight() - insertCardText->GetSize().GetHeight()) / 2 + BankyOffset));
  
    //Input pin sizer
    wxBoxSizer* inputPinSizer = new wxBoxSizer(wxHORIZONTAL);
    pincodeInputField->SetMaxSize(wxSize(200, -1));
    inputPinSizer->Add(pincodeInputField, wxSizerFlags().Proportion(1));
    inputPinSizer->AddSpacer(5);
    inputPinSizer->Add(enterPincodeButton);

    //Set the position of insertCardText
    int xOffset = 340; //Right (+) or left (-)
    int yOffset = 160; //Down (+) or up (-)
    insertCardText->SetPosition(wxPoint((whiteBoxPanel->GetSize().GetWidth() - insertCardText->GetSize().GetWidth()) / 2 + xOffset,
        (whiteBoxPanel->GetSize().GetHeight() - insertCardText->GetSize().GetHeight()) / 2 + yOffset));

    wxBoxSizer* mainContentSizer = new wxBoxSizer(wxVERTICAL);

    //Transaction Controls to white box panel sizer
    wxSizer* transactionSizer = new wxBoxSizer(wxVERTICAL);

    wxSize buttonSize(100, 40);
    balanceInquiryButton->SetMinSize(buttonSize);
    withdrawButton->SetMinSize(buttonSize);
    depositButton->SetMinSize(buttonSize);
    fundTransferButton->SetMinSize(buttonSize);
    changePinButton->SetMinSize(buttonSize);

    transactionSizer->Add(transactionText, wxSizerFlags().CenterHorizontal().Border(wxALL, 7));
    transactionSizer->Add(balanceInquiryButton, wxSizerFlags(0).Expand().Border(wxALL, 7));
    transactionSizer->Add(withdrawButton, wxSizerFlags(0).Expand().Border(wxALL, 7));
    transactionSizer->Add(depositButton, wxSizerFlags(0).Expand().Border(wxALL, 7));
    transactionSizer->Add(fundTransferButton, wxSizerFlags(0).Expand().Border(wxALL, 7));
    transactionSizer->Add(changePinButton, wxSizerFlags(0).Expand().Border(wxALL, 7));

    mainContentSizer->Add(transactionSizer, wxSizerFlags().Expand().Border(wxALL, 5));

    whiteBoxPanelSizer->Add(mainContentSizer, wxSizerFlags().Expand().Border(wxALL, 5));

    //Balance
    whiteBoxPanelSizer->Add(BalanceText, wxSizerFlags().CenterHorizontal().Border(wxALL, 130));
    whiteBoxPanelSizer->Add(ConfirmBalanceInquiryButton, wxSizerFlags(0).CenterHorizontal().Border(wxALL, 30));

    //Withraw
    WithrawInputField->SetMinSize(wxSize(300, 30));
    whiteBoxPanelSizer->Add(WithrawText, wxSizerFlags().CenterHorizontal().Border(wxALL, 60));
    whiteBoxPanelSizer->Add(WithrawInputField, wxSizerFlags().CenterHorizontal().Border(wxALL, 1));
    whiteBoxPanelSizer->Add(ConfirmWithrawButton, wxSizerFlags(0).CenterHorizontal().Border(wxALL, 30));

    //Deposit
    DepositInputField->SetMinSize(wxSize(300, 30));
    whiteBoxPanelSizer->Add(DepositText, wxSizerFlags().CenterHorizontal().Border(wxALL, 60));
    whiteBoxPanelSizer->Add(DepositInputField, wxSizerFlags().CenterHorizontal().Border(wxALL, 1));
    whiteBoxPanelSizer->Add(ConfirmDepositButton, wxSizerFlags(0).CenterHorizontal().Border(wxALL, 30));

    //FundTransfer
    receiverAccountInputField->SetMinSize(wxSize(300, 30));
    fundTransferAmountInputField->SetMinSize(wxSize(300, 30));  
    whiteBoxPanelSizer->Add(FundTransterText, wxSizerFlags().CenterHorizontal().Border(wxALL, 55));
    whiteBoxPanelSizer->Add(receiverAccountInputField, wxSizerFlags(0).CenterHorizontal().Border(wxALL, 5));
    whiteBoxPanelSizer->Add(fundTransferAmountInputField, wxSizerFlags(0).CenterHorizontal().Border(wxALL, 5));
    whiteBoxPanelSizer->Add(ConfirmFundTransferButton, wxSizerFlags(0).CenterHorizontal().Border(wxALL, 40));

    //ChangePincode
    oldPincodeInputField->SetMinSize(wxSize(300, 30));
    newPincodeInputField->SetMinSize(wxSize(300, 30));
    whiteBoxPanelSizer->Add(ChangePincodeText, wxSizerFlags().CenterHorizontal().Border(wxALL, 55));
    whiteBoxPanelSizer->Add(oldPincodeInputField, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));
    whiteBoxPanelSizer->Add(newPincodeInputField, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));
    whiteBoxPanelSizer->Add(ConfirmChangePincodeButton, wxSizerFlags(0).CenterHorizontal().Border(wxALL, 40));

    //Registration Controls

    wxSizer* registrationSizer = new wxBoxSizer(wxVERTICAL);
    //registrationSizer->AddSpacer(10);

    registrationSizer->Add(registrationText, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));
    //registrationSizer->AddSpacer(30);
    registrationSizer->Add(accountNameInputField, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));
    registrationSizer->Add(birthdayInputField, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));
    registrationSizer->Add(contactNumberInputField, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));
    registrationSizer->Add(pinCodeInputField, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));
   // registrationSizer->AddSpacer(50);
    registrationSizer->Add(registerButton, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));

    mainContentSizer->Add(registrationSizer, wxSizerFlags().Expand().Border(wxALL, 5));

    //Merging input pin controls to the white box panel sizer
    whiteBoxPanelSizer->AddSpacer(70);
    whiteBoxPanelSizer->Add(enterPincodeText, wxSizerFlags().CenterHorizontal().Border(wxALL, 10));
    whiteBoxPanelSizer->AddSpacer(70);
    whiteBoxPanelSizer->Add(inputPinSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 5));

    //WhiteBoxPanel to blackBorderSizer
    blackBorderSizer->AddSpacer(5); //Pababa yung adjustment, top spacer
    blackBorderSizer->Add(whiteBoxPanel, wxSizerFlags().Center().Proportion(0));
    blackBorderSizer->AddSpacer(5); //Pataas yung adjustment, bottom spacer

    //BlackBorderPanel to grayPanelSizer
    grayPanelSizer->AddSpacer(20);
    grayPanelSizer->Add(blackBorderPanel, wxSizerFlags().Center().Proportion(0));
    grayPanelSizer->AddSpacer(20);

    //GrayPanel to the main sizer
    mainSizer->Add(grayPanel, wxSizerFlags().Expand().Proportion(1).Border(wxALL, 10));

    //Set main sizer on the panel
    panel->SetSizer(mainSizer);

    panel->Layout();
    mainSizer->Fit(panel);
}

void MainFrame::BindEventHandlers()
{
    this->Bind(wxEVT_TIMER, &MainFrame::OnTimer, this);

    balanceInquiryButton->Bind(wxEVT_BUTTON, &MainFrame::OnBalanceInquiryButtonClicked, this);
    registerButton->Bind(wxEVT_BUTTON, &MainFrame::OnRegisterButtonClicked, this);
    enterPincodeButton->Bind(wxEVT_BUTTON, &MainFrame::OnEnterPincodeButton, this);

    // Bind focus and unfocus events for the text controls
    accountNameInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    accountNameInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    birthdayInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    birthdayInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    contactNumberInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    contactNumberInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    pinCodeInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    pinCodeInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    //Transaction controls 

    WithrawInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    WithrawInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    DepositInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    DepositInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    receiverAccountInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    receiverAccountInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    fundTransferAmountInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    fundTransferAmountInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    oldPincodeInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    oldPincodeInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    newPincodeInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    newPincodeInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    //Bind Window Close Event and Save to .csv file
    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClose, this);

    withdrawButton->Bind(wxEVT_BUTTON, &MainFrame::OnWithrawButtonClicked, this);
    depositButton->Bind(wxEVT_BUTTON, &MainFrame::OnDepositButtonClicked, this);
    fundTransferButton->Bind(wxEVT_BUTTON, &MainFrame::OnFundTransferButtonClicked, this);
    changePinButton->Bind(wxEVT_BUTTON, &MainFrame::OnChangePincodeButtonClicked, this);

    ConfirmBalanceInquiryButton->Bind(wxEVT_BUTTON, &MainFrame::OnConfirmBalanceInquiryButtonClicked, this);
    ConfirmWithrawButton->Bind(wxEVT_BUTTON, &MainFrame::OnConfirmWithrawButtonClicked, this);
    ConfirmDepositButton->Bind(wxEVT_BUTTON, &MainFrame::OnConfirmDepositButtonClicked, this);
    ConfirmFundTransferButton->Bind(wxEVT_BUTTON, &MainFrame::OnConfirmFundTransferButtonClicked, this);
    ConfirmChangePincodeButton->Bind(wxEVT_BUTTON, &MainFrame::OnConfirmChangePincodeButonClicked, this);

    registerButton->Bind(wxEVT_BUTTON, &MainFrame::OnRegisterButtonClicked, this);
}

void MainFrame::OnTimer(wxTimerEvent& evt)
{
   
   

    if (IsFlashDriveInserted('D'))
    {
        timer->Stop();
        OnFlashDriveInserted();
    }

    else
    {
        ShowInsertCardText(true);
        ShowEnterPincode(false);
        ShowRegistrationControls(false);
        ShowTransactionControls(false);

        panel->Layout();
    }
}

void MainFrame::OnFlashDriveInserted()
{
    if (bank.isCardRegistered())
    {
        ShowEnterPincode(true);
        ShowInsertCardText(false);
        ShowRegistrationControls(false);
        ShowTransactionControls(false);
    }
    else
    {
        ShowRegistrationControls(true);
        ShowInsertCardText(false);
        ShowEnterPincode(false);
        ShowTransactionControls(false);

    }
    panel->Layout();
}

void MainFrame::OnRegisterButtonClicked(wxCommandEvent& evt)
{
    wxString name = accountNameInputField->GetValue();
    wxString birthday = birthdayInputField->GetValue();
    wxString contactNumber = contactNumberInputField->GetValue();
    wxString pincode = pinCodeInputField->GetValue();

    // Define your placeholders
    wxString namePlaceholder = "Name: ";
    wxString birthdayPlaceholder = "Birthday (MM-DD-YYYY): ";
    wxString contactNumberPlaceholder = "Contact Number (00000000000): ";
    wxString pinCodePlaceholder = "PIN Code (000000): ";

    // Basic validation (optional)
    if (name.IsEmpty() || name == namePlaceholder || birthday.IsEmpty() || birthday == birthdayPlaceholder ||
        contactNumber == contactNumberPlaceholder || contactNumber.IsEmpty() || pincode.IsEmpty() || pincode == pinCodePlaceholder)
    {
        wxMessageBox("Please fill in all the fields.", "Registration Error", wxOK | wxICON_ERROR);
        return;
    }

    if (name.length() > 30)
    {
        wxMessageBox("Name must not exceed 30 characters.", "Registration Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!wxRegEx("^(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])-(\\d{4})$").Matches(birthday))
    {
        wxMessageBox("Birthday must be in the format MM-DD-YYYY.", "Registration Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!wxRegEx("^(0)\\d{10}$").Matches(contactNumber))
    {
        wxMessageBox("Contact Number must be in the format 00000000000 (starts at 0 with 11 digits).", "Registration Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!wxRegEx("^\\d{6}$").Matches(pincode))
    {
        wxMessageBox("PIN Code must be exactly 6 digits.", "Registration Error", wxOK | wxICON_ERROR);
        return;
    }

    string accountNumber = bank.generateAccountNumber(); // Increment and format

    // Initial balance for new accounts (0.0 for new registrations)
    double initialBalance = 5000.0;
    string lastDigit = bank.getSignificantDigits(accountNumber);
    string hashedPin = bank.hashPinCode(pincode.ToStdString(), lastDigit);

    // Create an Account object
    Account newAccount(accountNumber,
        name.ToStdString(),
        hashedPin,
        birthday.ToStdString(),
        contactNumber.ToStdString(),
        initialBalance);

    if (bank.accountDuplicationChecker(newAccount.name, newAccount.contactNumber))
    {
        wxMessageBox("Account Exist! Registration Denied.",
            "Registration Error", wxOK | wxICON_ERROR);
    }
    else
    {
        // Add the new account to the bank
        bank.add(newAccount);

        // Check if flash drive is inserted before saving the account
        SaveAccountToFlashDrive(newAccount);

        bank.saveAllAccounts();
    }

    // Reset the input fields
    accountNameInputField->SetValue(namePlaceholder);
    birthdayInputField->SetValue(birthdayPlaceholder);
    contactNumberInputField->SetValue(contactNumberPlaceholder);
    pinCodeInputField->SetValue(pinCodePlaceholder);

    // Go to Pincode
    ShowEnterPincode(true);
    ShowRegistrationControls(false);
}

void MainFrame::OnWindowClose(wxCloseEvent& event)
{
    bank.saveAllAccounts();
    event.Skip();
}

void MainFrame::OnFocus(wxFocusEvent& event)
{
    wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
    if (textCtrl)
    {
        wxString placeholder;
        if (textCtrl == accountNameInputField)
            placeholder = "Name: ";
        else if (textCtrl == birthdayInputField)
            placeholder = "Birthday (MM-DD-YYYY): ";
        else if (textCtrl == contactNumberInputField)
            placeholder = "Contact Number (+63 000-000-0000): ";
        else if (textCtrl == pinCodeInputField)
            placeholder = "PIN Code (000000): ";
        else if (textCtrl == WithrawInputField)
            placeholder = "Withdraw amount: ";
        else if (textCtrl == DepositInputField)
            placeholder = "Deposit amount: ";
        else if (textCtrl == receiverAccountInputField)
            placeholder = "Recipient Account Number: ";
        else if (textCtrl == fundTransferAmountInputField)
            placeholder = "Transfer amount: ";
        else if (textCtrl == oldPincodeInputField)
            placeholder = "Old PIN Code: ";
        else if (textCtrl == newPincodeInputField)
            placeholder = "New PIN Code: ";

        if (textCtrl->GetValue() == placeholder)
        {
            textCtrl->SetValue("");
        }
    }
    event.Skip();
}


void MainFrame::OnUnfocus(wxFocusEvent& event)
{
    wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
    if (textCtrl)
    {
        wxString placeholder;
        if (textCtrl == accountNameInputField)
            placeholder = "Name: ";
        else if (textCtrl == birthdayInputField)
            placeholder = "Birthday (MM-DD-YYYY): ";
        else if (textCtrl == contactNumberInputField)
            placeholder = "Contact Number (+63 000-000-0000): ";
        else if (textCtrl == pinCodeInputField)
            placeholder = "PIN Code (000000): ";
        else if (textCtrl == WithrawInputField)
            placeholder = "Withdraw amount: ";
        else if (textCtrl == DepositInputField)
            placeholder = "Deposit amount: ";
        else if (textCtrl == receiverAccountInputField)
            placeholder = "Recipient Account Number: ";
        else if (textCtrl == fundTransferAmountInputField)
            placeholder = "Transfer amount: ";
        else if (textCtrl == oldPincodeInputField)
            placeholder = "Old PIN Code: ";
        else if (textCtrl == newPincodeInputField)
            placeholder = "New PIN Code: ";

        if (textCtrl->GetValue().IsEmpty())
        {
            textCtrl->SetValue(placeholder);
        }
    }
    event.Skip();
}


void MainFrame::SaveAccountToFlashDrive(const Account& newAccount, char driveLetter)
{
    if (IsFlashDriveInserted('D'))
    {
        char lastdigit = newAccount.accountNumber.back();  // Get the last character

      /*  string hashedPin = bank.hashPinCode(newAccount.pincode, lastdigit);*/
        string filepath = "D:\\account.txt";

        // Open the file on the flash drive for writing
        ofstream outFile(filepath, ios::app);  // Use append mode to prevent overwriting
        if (outFile.is_open())
        {
            outFile << newAccount.accountNumber << endl // First line: account number
                << newAccount.pincode << endl;
            outFile.close();

            // Notify the user of the successful save
            wxMessageBox("Account Registered and saved to the flash drive!", "Success", wxOK | wxICON_INFORMATION);

        }
        else
        {
            wxMessageBox("Error: Unable to write to the flash drive.", "Error", wxOK | wxICON_ERROR);
        }
    }
    else
    {
        wxMessageBox("Flash drive not detected. Account registered, but not saved to the flash drive.", "Registration Successful", wxOK | wxICON_WARNING);
    }
}

bool MainFrame::IsFlashDriveInserted(char driveLetter)
{
    wstring drivePath = wstring(1, driveLetter) + L":\\";

    UINT driveType = GetDriveType(drivePath.c_str());

    // DRIVE_REMOVABLE represents removable drives like USB flash drives
    return (driveType == DRIVE_REMOVABLE);
}

void MainFrame::ShowInsertCardText(bool show)
{
    insertCardText->Show(show);
}

void MainFrame::ShowEnterPincode(bool show)
{
    enterPincodeText->Show(show);
    pincodeInputField->Show(show);
    enterPincodeButton->Show(show);
}

void MainFrame::ShowRegistrationControls(bool show)
{
    registrationText->Show(show);
    accountNameInputField->Show(show);
    birthdayInputField->Show(show);
    contactNumberInputField->Show(show);
    pinCodeInputField->Show(show);
    registerButton->Show(show);
}

void MainFrame::ShowTransactionControls(bool show)
{
    transactionText->Show(show);
    balanceInquiryButton->Show(show);
    withdrawButton->Show(show);
    depositButton->Show(show);
    fundTransferButton->Show(show);
    changePinButton->Show(show);

    ShowBalanceInquiryControls(false);
    ShowWithrawTransactionControls(false);
    ShowFundTransferTransactionControls(false);
    ShowChangePincodeTransactionControls(false);

    if (show) {
        panel->Layout();
    }
}

void MainFrame::ShowBalanceInquiryControls(bool show)
{
    BalanceText->Show(show);
    ConfirmBalanceInquiryButton->Show(show);
}

void MainFrame::ShowWithrawTransactionControls(bool show)
{
    WithrawText->Show(show);
    WithrawInputField->Show(show);
    ConfirmWithrawButton->Show(show);
}

void MainFrame::ShowDepositTransactionControls(bool show)
{
    DepositText->Show(show);
    DepositInputField->Show(show);
    ConfirmDepositButton->Show(show);
}

void MainFrame::ShowFundTransferTransactionControls(bool show)
{
    FundTransterText->Show(show);
    receiverAccountInputField->Show(show);
    fundTransferAmountInputField->Show(show);
    ConfirmFundTransferButton->Show(show);
}

void MainFrame::ShowChangePincodeTransactionControls(bool show)
{
    ChangePincodeText->Show(show);
    oldPincodeInputField->Show(show);
    newPincodeInputField->Show(show);
    ConfirmChangePincodeButton->Show(show);
}

void MainFrame::OnBalanceInquiryButtonClicked(wxCommandEvent& evt)
{
    ShowTransactionControls(false);
    ShowBalanceInquiryControls(true);
    double balance = bank.currentAccount.balance;

    BalanceText->SetLabel(wxString::Format("Current Balance: %.2f", balance));

    BalanceText->Show(true);
    panel->Layout();
}

void MainFrame::OnConfirmBalanceInquiryButtonClicked(wxCommandEvent& evt)
{
    ShowBalanceInquiryControls(false);
    ShowTransactionControls(true);
    panel->Layout();
}

void MainFrame::OnWithrawButtonClicked(wxCommandEvent& evt) {
    ShowTransactionControls(false);
    ShowWithrawTransactionControls(true);
    panel->Layout();
}

void MainFrame::OnConfirmWithrawButtonClicked(wxCommandEvent& evt) {

    string amountStr = string(WithrawInputField->GetValue().mb_str());
    if (amountStr.empty()) {
        wxMessageBox("Input Empty", "Error", wxOK | wxICON_INFORMATION);
    }
    try {
        double amount = stod(amountStr);
        bank.withraw(amount);
        wxMessageBox("Successful Transaction", "Success", wxOK | wxICON_INFORMATION);
    }
    catch(const invalid_argument& i){
        wxMessageBox(i.what(), "Error", wxOK | wxICON_ERROR);
    }
    catch (const out_of_range& r) {
        wxMessageBox(r.what(), "Error", wxOK | wxICON_ERROR);
    }
    catch (const exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
    WithrawInputField->Clear();
    ShowWithrawTransactionControls(false);
    askForAnotherTrans(evt);
}

void MainFrame::OnDepositButtonClicked(wxCommandEvent& evt)
{
    ShowTransactionControls(false);
    ShowDepositTransactionControls(true);
    panel->Layout();
}

void MainFrame::OnConfirmDepositButtonClicked(wxCommandEvent& evt)
{

    string amountStr = string(DepositInputField->GetValue().mbc_str());
    if (amountStr.empty()) {
        wxMessageBox("Input Empty", "Error", wxOK | wxICON_INFORMATION);
    }
    try {
        double amount = stod(amountStr);
        bank.deposit(amount);

        wxMessageBox("Successful Transaction", "Success", wxICON_INFORMATION);
    }
    catch (const invalid_argument& i) {
        wxMessageBox(i.what(), "Error", wxOK | wxICON_ERROR);
    }
    catch (const exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
    DepositInputField->Clear();
    ShowDepositTransactionControls(false);
    ShowTransactionControls(true);
    askForAnotherTrans(evt);

}

void MainFrame::OnFundTransferButtonClicked(wxCommandEvent& evt)
{
    ShowTransactionControls(false);
    ShowFundTransferTransactionControls(true);
    panel->Layout();
}

void MainFrame::OnConfirmFundTransferButtonClicked(wxCommandEvent& evt)
{
    string amountStr = string(fundTransferAmountInputField->GetValue().mbc_str());
    string receiverAccountNumber = string(receiverAccountInputField->GetValue().mb_str());
    
    if (amountStr.empty() || receiverAccountNumber.empty()) {
        wxMessageBox("Input Empty", "Error", wxOK | wxICON_INFORMATION);
    }

    try
    {
        double amount = stod(amountStr);
        bank.fundTransfer(receiverAccountNumber, amount);
        
        wxMessageBox(wxString::Format("Successfully Transaction", "Successful", wxOK | wxICON_INFORMATION));
    }
    catch (const invalid_argument& i) {
        wxMessageBox(i.what(), "Error", wxOK | wxICON_ERROR);
    }
    catch (const out_of_range& r) {
        wxMessageBox(r.what(), "Error", wxOK | wxICON_ERROR);
    }
    catch (const exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
    ShowFundTransferTransactionControls(false);
    ShowTransactionControls(true);
    askForAnotherTrans(evt);

}

void MainFrame::OnChangePincodeButtonClicked(wxCommandEvent& evt)
{
    ShowTransactionControls(false);
    ShowChangePincodeTransactionControls(true);
    panel->Layout();
}

void MainFrame::OnEnterPincodeButton(wxCommandEvent& evt)
{
    wxString pincode = pincodeInputField->GetValue();
    if (!wxRegEx("^\\d{6}$").Matches(pincode))
    {
        wxMessageBox("Invalid PIN Code. Please enter exactly 6 digits.", "PIN Error", wxOK | wxICON_ERROR);
        return;
    }
    // Open the flash drive file to retrieve the account number from the first line
    string flashDriveFilePath = string(1, bank.getDriveLetter()) + ":\\account.txt";
    ifstream flashDriveFile(flashDriveFilePath);

    if (!flashDriveFile.is_open())
    {
        wxMessageBox("Error: Unable to read the flash drive.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    // Read the account number from the first line of the flash drive
    string accountNumber;
    getline(flashDriveFile, accountNumber);
    flashDriveFile.close();

    if (accountNumber.empty())
    {
        wxMessageBox("Error: Account number not found on the flash drive.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    string lastDigitOfAccNumber = bank.getSignificantDigits(accountNumber);
    
    string hashedPin = bank.hashPinCode(pincode.ToStdString(), lastDigitOfAccNumber);

  
    // Check if a matching account was found
    if (bank.accountExists(hashedPin))
    {
        wxMessageBox("PIN verified successfully!", "Success", wxOK | wxICON_INFORMATION);

        isLoggedIn = true;
        bank.currentAccount = bank.searchAccount(accountNumber);
        // Proceed to show transaction options if PIN matched
        ShowTransactionControls(true);
        ShowEnterPincode(false);
        panel->Layout();
    }
    else
    {
        wxMessageBox("Invalid PIN Code. Please try again.", "PIN Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnTransactionCompleted()
{
    // Reset the isLoggedIn flag so the app can go back to the "Insert Card" state
    isLoggedIn = false;

    ShowInsertCardText(true);
    ShowEnterPincode(false);
    ShowRegistrationControls(false);
    ShowTransactionControls(false);
    panel->Layout();
}

void MainFrame::OnConfirmChangePincodeButonClicked(wxCommandEvent& evt)
{
    string oldPin = string(oldPincodeInputField->GetValue().mb_str());
    string newPin = string(newPincodeInputField->GetValue().mb_str());

    string lastDigitOfAccNumber = bank.getSignificantDigits(bank.currentAccount.accountNumber);
    string hashedOldPin = bank.hashPinCode(oldPin, lastDigitOfAccNumber);
    string hashedNewPin = bank.hashPinCode(newPin, lastDigitOfAccNumber);
    
    try {
        bank.changePincode(hashedOldPin, hashedNewPin);
        wxMessageBox("Successfully Changed PIN", "Success", wxOK | wxICON_INFORMATION);
    }
    catch (const invalid_argument& i) {
        wxMessageBox(i.what(), "Error", wxOK | wxICON_ERROR);
    }
    catch (const out_of_range& r) {
        wxMessageBox(r.what(), "Error", wxOK | wxICON_ERROR);
    }
    catch (const exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
    ShowChangePincodeTransactionControls(false);
    ShowTransactionControls(true);
    askForAnotherTrans(evt);
}

void MainFrame::askForAnotherTrans(wxCommandEvent& evt)
{
    int respond = wxMessageBox("Do you want to make another transaction?", "Another Transaction", wxYES_NO | wxICON_INFORMATION);

    if (respond == wxYES) {
        ShowTransactionControls(true);
        ShowBalanceInquiryControls(false);
        ShowWithrawTransactionControls(false);
        ShowDepositTransactionControls(false);
        ShowFundTransferTransactionControls(false);
        ShowChangePincodeTransactionControls(false);
    }
    else {
        Close(true);
    }
}
 