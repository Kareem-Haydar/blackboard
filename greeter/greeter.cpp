#include <cstring>
#include <iostream>
#include <string>
#include <security/pam_appl.h>
#include <unistd.h>

using namespace std;

// PAM conversation function
int pam_conversation(int num_msg, const struct pam_message** msg,
                     struct pam_response** resp, void* appdata_ptr) {
    if (num_msg <= 0) return PAM_CONV_ERR;

    struct pam_response* reply = new pam_response[num_msg];
    string* password = static_cast<string*>(appdata_ptr);

    for (int i = 0; i < num_msg; ++i) {
        if (msg[i]->msg_style == PAM_PROMPT_ECHO_OFF) {
            reply[i].resp = strdup(password->c_str());
            reply[i].resp_retcode = 0;
        } else {
            reply[i].resp = nullptr;
            reply[i].resp_retcode = 0;
        }
    }

    *resp = reply;
    return PAM_SUCCESS;
}

bool authenticate(const string& username, const string& password) {
    pam_handle_t* pamh = nullptr;
    struct pam_conv conv = { pam_conversation, (void*)&password };

    int status = pam_start("lightdm", username.c_str(), &conv, &pamh);
    if (status != PAM_SUCCESS) return false;

    status = pam_authenticate(pamh, 0);
    if (status == PAM_SUCCESS)
        status = pam_acct_mgmt(pamh, 0);  // check if account is valid

    pam_end(pamh, status);
    return status == PAM_SUCCESS;
}

int main() {
    string username, password;

    cout << "Welcome to Linux" << endl;
    cout << "----------------" << endl;
    cout << "Username: ";
    getline(cin, username);

    cout << "Password: ";
    system("stty -echo"); // turn off input echoing
    getline(cin, password);
    system("stty echo");
    cout << endl;

    if (authenticate(username, password)) {
        cout << "Login successful." << endl;
        return 0;  // LightDM expects 0 on success
    } else {
        cout << "Login failed." << endl;
        return 1;
    }
}


