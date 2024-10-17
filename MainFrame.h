#pragma once
#include <wx/wx.h>
#include "classes.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
	
private:
	Account adminAccount;
	Bank bank;
	bool isLoggedIn;

	void CreateControls();
	void SetupSizers();
	void BindEventHandlers();

	void OnTimer(wxTimerEvent& evt);
	void OnFlashDriveInserted();
	void OnRegisterButtonClicked(wxCommandEvent& evt);
	void OnWindowClose(wxCloseEvent& event);

	void OnFocus(wxFocusEvent& event);
	void OnUnfocus(wxFocusEvent& event);

	void SaveAccountToFlashDrive(const Account& newAccount, char driveLetter = 'D');
	bool IsFlashDriveInserted(char driveLetter);
	
	
	void ShowInsertCardText(bool show);
	void ShowEnterPincode(bool show);
	void ShowRegistrationControls(bool show);
	void ShowTransactionControls(bool show);
	void ShowBalanceInquiryControls(bool show);
	void ShowWithrawTransactionControls(bool show);
	void ShowDepositTransactionControls(bool show);
	void ShowFundTransferTransactionControls(bool show);
	void ShowChangePincodeTransactionControls(bool show);

	void OnBalanceInquiryButtonClicked(wxCommandEvent& evt);
	void OnEnterPincodeButton(wxCommandEvent& evt);
	void OnTransactionCompleted();
	void OnConfirmBalanceInquiryButtonClicked(wxCommandEvent& evt);

	void OnWithrawButtonClicked(wxCommandEvent& evt);
	void OnConfirmWithrawButtonClicked(wxCommandEvent& evt);

	void OnDepositButtonClicked(wxCommandEvent& evt);
	void OnConfirmDepositButtonClicked(wxCommandEvent& evt);

	void OnFundTransferButtonClicked(wxCommandEvent& evt);
	void OnConfirmFundTransferButtonClicked(wxCommandEvent& evt);
	
	void OnChangePincodeButtonClicked(wxCommandEvent& evt);
	void OnConfirmChangePincodeButonClicked(wxCommandEvent& evt);

	void askForAnotherTrans(wxCommandEvent& evt);

	wxPanel* panel;
	wxTimer* timer;

	wxStaticText* insertCardText;
	wxStaticText* bankName;
	wxStaticText* enterPincodeText;
	wxTextCtrl* pincodeInputField;
	wxButton* enterPincodeButton;

	wxStaticText* registrationText;
	wxTextCtrl* accountNameInputField;
	wxTextCtrl* birthdayInputField;
	wxTextCtrl* contactNumberInputField;
	wxTextCtrl* pinCodeInputField;
	wxButton* registerButton;
	
	wxStaticText* transactionText;
	wxButton* balanceInquiryButton; 
	wxButton* withdrawButton;
	wxButton* depositButton;
	wxButton* fundTransferButton; 
	wxButton* changePinButton; 

	wxStatusBar* statusBar;

	// Balance Inquiry



	wxStaticText* BalanceText;
	wxStaticText* WithrawText;
	wxStaticText* DepositText;
	

	wxTextCtrl* WithrawInputField;
	wxTextCtrl* DepositInputField;


	wxButton* ConfirmBalanceInquiryButton;
	wxButton* ConfirmWithrawButton;
	wxButton* ConfirmDepositButton;

	
	wxStaticText* FundTransterText;
	wxTextCtrl* receiverAccountInputField;
	wxTextCtrl* fundTransferAmountInputField;
	wxButton* ConfirmFundTransferButton;
	

	wxStaticText* ChangePincodeText;
	wxTextCtrl* oldPincodeInputField;
	wxTextCtrl* newPincodeInputField;
	wxButton* ConfirmChangePincodeButton;

};

