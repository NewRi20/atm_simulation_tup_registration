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
    wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0, 20)));

    panel = new wxPanel(this);
    panel->SetFont(mainFont);

    timer = new wxTimer(this, wxID_ANY);
    timer->Start(2000);
    
    insertCardText = new wxStaticText(panel, wxID_ANY, "Please insert card...");
    insertCardText->SetFont(headlineFont);

    // Enter PIN Code Field
    enterPincodeText = new wxStaticText(panel, wxID_ANY, "Enter PIN Code");
    enterPincodeText->SetFont(headlineFont);
    pincodeInputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    enterPincodeButton = new wxButton(panel, wxID_ANY, "Enter PIN Code", wxDefaultPosition, wxDefaultSize);

    // Registration Controlsz`
    registrationText = new wxStaticText(panel, wxID_ANY, "Register for an Account");
    registrationText->SetFont(headlineFont);

    accountNameInputField = new wxTextCtrl(panel, wxID_ANY, "Name: ", wxDefaultPosition, wxDefaultSize); 
    birthdayInputField = new wxTextCtrl(panel, wxID_ANY, "Birthday (MM-DD-YYYY): ", wxDefaultPosition, wxDefaultSize);
    contactNumberInputField = new wxTextCtrl(panel, wxID_ANY, "Contact Number(00000000000) : ", wxDefaultPosition, wxDefaultSize);
    pinCodeInputField = new wxTextCtrl(panel, wxID_ANY, "PIN Code (000000): ", wxDefaultPosition, wxDefaultSize); 

    registerButton = new wxButton(panel, wxID_ANY, "Register");
  
    // Transaction Controls
    transactionText = new wxStaticText(panel, wxID_ANY, "Choose a Transaction");
    transactionText->SetFont(headlineFont);
    balanceInquiryButton = new wxButton(panel, wxID_ANY, "Balance Inquiry");
    withdrawButton = new wxButton(panel, wxID_ANY, "Withdraw");
    depositButton = new wxButton(panel, wxID_ANY, "Deposit");
    fundTransferButton = new wxButton(panel, wxID_ANY, "Fund Transfer");
    changePinButton = new wxButton(panel, wxID_ANY, "Change PIN Code");


    //                  INSIDE TRANS
    // Balance Inquiry Controls
    //BalanceText = new wxStaticText(panel, wxID_ANY, "Current Balance");

    // balance
    BalanceText = new wxStaticText(panel, wxID_ANY, "Current Balance");
    BalanceText->SetFont(headlineFont);
    ConfirmBalanceInquiryButton = new wxButton(panel, wxID_ANY, "Confirm");

    //withraw Controls
    WithrawText = new wxStaticText(panel, wxID_ANY, "Withraw Transaction", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
    WithrawText->SetFont(headlineFont);
    WithrawInputField = new wxTextCtrl(panel, wxID_ANY, "Amount", wxDefaultPosition, wxDefaultSize);
    ConfirmWithrawButton = new wxButton(panel, wxID_ANY, "Confirm");

    //  DEPOSIT CONTROLS
    DepositText = new wxStaticText(panel, wxID_ANY, "Deposit Transaction", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
    DepositText->SetFont(headlineFont);
    DepositInputField = new wxTextCtrl(panel, wxID_ANY, "Amount", wxDefaultPosition, wxDefaultSize);
    ConfirmDepositButton = new wxButton(panel, wxID_ANY, "Confirm");

    //  FUND TRANSFER CONTROLS
    FundTransterText = new wxStaticText(panel, wxID_ANY, "Fund Transfer", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
    FundTransterText->SetFont(headlineFont);
    receiverAccountInputField = new wxTextCtrl(panel, wxID_ANY, "Recipient Account Number", wxDefaultPosition, wxDefaultSize);
    fundTransferAmountInputField = new wxTextCtrl(panel, wxID_ANY, "Amount", wxDefaultPosition, wxDefaultSize);
    ConfirmFundTransferButton = new wxButton(panel, wxID_ANY, "Confirm Transfer");

    //  CHANGE PIN CODE CONTROLS
    ChangePincodeText = new wxStaticText(panel, wxID_ANY, "Change PinCode", wxPoint(0, 22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
    ChangePincodeText->SetFont(headlineFont);
    oldPincodeInputField = new wxTextCtrl(panel, wxID_ANY, "Old PinCode", wxDefaultPosition, wxDefaultSize);
    newPincodeInputField = new wxTextCtrl(panel, wxID_ANY, "New PinCode", wxDefaultPosition, wxDefaultSize);
    ConfirmChangePincodeButton = new wxButton(panel, wxID_ANY, "Confirm Changes");



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

    mainSizer->Add(insertCardText, wxSizerFlags().Center());

    // Add a Box Sizer for PIN Code field and button
    wxBoxSizer* inputPinSizer = new wxBoxSizer(wxHORIZONTAL);
    inputPinSizer->Add(pincodeInputField, wxSizerFlags().Proportion(1));
    inputPinSizer->AddSpacer(5);
    inputPinSizer->Add(enterPincodeButton);

    mainSizer->Add(enterPincodeText, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(inputPinSizer, wxSizerFlags().Expand());
    
    // Add Registration Controls to Sizer
    mainSizer->Add(registrationText, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(accountNameInputField, wxSizerFlags().Expand());
    mainSizer->Add(birthdayInputField, wxSizerFlags().Expand());
    mainSizer->Add(contactNumberInputField, wxSizerFlags().Expand());
    mainSizer->Add(pinCodeInputField, wxSizerFlags().Expand());
    mainSizer->Add(registerButton, wxSizerFlags().Expand());
    
    // Add Transaction Controls to Sizer
    mainSizer->Add(transactionText, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(balanceInquiryButton, wxSizerFlags().Expand());
    mainSizer->Add(withdrawButton, wxSizerFlags().Expand());
    mainSizer->Add(depositButton, wxSizerFlags().Expand());
    mainSizer->Add(fundTransferButton, wxSizerFlags().Expand());
    mainSizer->Add(changePinButton, wxSizerFlags().Expand());

    // Add Balance Inquiry Controls to Sizer
    mainSizer->Add(BalanceText, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(ConfirmBalanceInquiryButton, wxSizerFlags().Expand().Border(wxALL, 10));

    mainSizer->Add(WithrawText, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(WithrawInputField, wxSizerFlags().Expand());
    mainSizer->Add(ConfirmWithrawButton, wxSizerFlags().Expand().Border(wxALL, 10));

    mainSizer->Add(DepositText, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(DepositInputField, wxSizerFlags().Expand());
    mainSizer->Add(ConfirmDepositButton, wxSizerFlags().Expand().Border(wxALL, 10));

    mainSizer->Add(FundTransterText, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(receiverAccountInputField, wxSizerFlags().Expand());
    mainSizer->Add(fundTransferAmountInputField, wxSizerFlags().Expand());
    mainSizer->Add(ConfirmFundTransferButton, wxSizerFlags().Expand().Border(wxALL, 10));

    mainSizer->Add(ChangePincodeText, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(oldPincodeInputField, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(newPincodeInputField, wxSizerFlags().CenterHorizontal());
    mainSizer->Add(ConfirmChangePincodeButton, wxSizerFlags().CenterHorizontal());


    // Outer Sizer for Border 
    wxGridSizer* outerSizer = new wxGridSizer(1);
    outerSizer->Add(mainSizer, wxSizerFlags(1).Border(wxALL, 25).Expand());

    panel->SetSizer(outerSizer);
    outerSizer->SetSizeHints(this);
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

    //Bind Window Close Event and Save to .csvfile
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

    // Bind focus and unfocus events for the text controls
    accountNameInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    accountNameInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    birthdayInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    birthdayInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    contactNumberInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    contactNumberInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    pinCodeInputField->Bind(wxEVT_SET_FOCUS, &MainFrame::OnFocus, this);
    pinCodeInputField->Bind(wxEVT_KILL_FOCUS, &MainFrame::OnUnfocus, this);

    this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClose, this);
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
