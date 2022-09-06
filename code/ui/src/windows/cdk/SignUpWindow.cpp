//
// Created by denis on 06.08.2022.
//

#include "windows/cdk/SignUpWindow.h"
#include "cdk/cdk.h"
#include "User.h"
#include <cstring>

class polytour::ui::cdk::SignUpWindow::Impl {
public:

    explicit Impl(const std::shared_ptr<ICoordinator>& coordinator);
    ~Impl() { destroy(); }
    void destroy();
    void init();

    typedef struct UserData {
        SScreen* cdk_screen;
        std::weak_ptr<polytour::ui::ICoordinator> coordinator;
        char* nickname;
        char* password;
        char* name;
        char* email;
        char* surname;
        char* age;
        Impl* _impl;
    } UserData;

private:

    SEntry* createEntry(int y, const char* label, const char* name);
    static int yNextTo(SEntry* entry);
    static int yPrevFrom(SEntry* entry);
    static int deactivateObj(EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED);
    static polytour::transport::User userDataToUser(const UserData& userdata);

    std::weak_ptr<ICoordinator> _pCoordinator;
    std::function<void()> _callback;

    SScreen* cdkScreen;
    SEntry* nickEntry;
    SEntry* passwordEntry;
    SEntry* nameEntry;
    SEntry* emailEntry;
    SEntry* surnameEntry;
    SEntry* ageEntry;

    SLabel* logo;
    SLabel* windowName;

    SButton* cancelButton;
    SButton* signUpButton;
    SButton* signInButton;

    bool isDestroyed = false;

    UserData _userdata;

    static BINDFN_PROTO (activateEntry);
    static BINDFN_PROTO (activateButton);
    static BINDFN_PROTO (signUp);
    static BINDFN_PROTO (cancel);
    static BINDFN_PROTO (toSignIn);
};

polytour::ui::cdk::SignUpWindow::Impl::Impl(const std::shared_ptr<ICoordinator> &coordinator):
_pCoordinator(coordinator) {}

SEntry *polytour::ui::cdk::SignUpWindow::Impl::createEntry(int y, const char* label, const char* name) {
    auto result = newCDKEntry (cdkScreen, CENTER, y, label, name, A_NORMAL, '_', vMIXED,
                               40, 256, 256, TRUE, FALSE);
    setCDKEntryULChar(result, ACS_LTEE);
    setCDKEntryURChar(result, ACS_RTEE);
    setCDKEntryLLChar(result, ACS_LTEE);
    setCDKEntryLRChar(result, ACS_RTEE);
}

int polytour::ui::cdk::SignUpWindow::Impl::yNextTo(SEntry* entry) {
    return getbegy(entry->win) + entry->boxHeight - 1;
}

int polytour::ui::cdk::SignUpWindow::Impl::yPrevFrom(SEntry *entry) {
    return getbegy(entry->win) - entry->boxHeight - 1;
}

void polytour::ui::cdk::SignUpWindow::Impl::destroy() {
    if (isDestroyed)
        return;
    destroyCDKEntry(nickEntry);
    destroyCDKEntry(passwordEntry);
    destroyCDKEntry(nameEntry);
    destroyCDKEntry(emailEntry);
    destroyCDKEntry(surnameEntry);
    destroyCDKEntry(ageEntry);

    destroyCDKLabel(logo);
    destroyCDKLabel(windowName);

    destroyCDKButton(cancelButton);
    destroyCDKButton(signInButton);
    destroyCDKButton(signUpButton);

    destroyCDKScreen (cdkScreen);
    endCDK ();
    isDestroyed = true;
}

int polytour::ui::cdk::SignUpWindow::Impl::activateEntry(EObjectType objType, void * obj, void * clientData, chtype key) {
    if (!deactivateObj(objType, obj)) return (FALSE);

    auto *entry = (CDKENTRY *)clientData;
    (void) activateCDKEntry(entry, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::SignUpWindow::Impl::activateButton(EObjectType objType, void * obj, void * clientData, chtype key) {
    if (!deactivateObj(objType, obj)) return (FALSE);

    auto *buttonbox = (CDKBUTTON *)clientData;
    (void)activateCDKButton(buttonbox, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::SignUpWindow::Impl::deactivateObj(EObjectType cdktype, void *object) {
    chtype esc = KEY_ESC;
    if (cdktype == vBUTTON)
        (void) activateCDKButton((CDKBUTTON*)object, &esc);
    else if (cdktype == vENTRY)
        (void) activateCDKEntry((CDKENTRY*) object, &esc);
    else return (FALSE);
    return (TRUE);
}

int polytour::ui::cdk::SignUpWindow::Impl::cancel(EObjectType objType, void * obj, void *data, chtype key) {
    auto* userData = (UserData*)data;
    if (!deactivateObj(objType, obj)) return (FALSE);
    userData->_impl->_callback = [](){};
    return (TRUE);
}

int polytour::ui::cdk::SignUpWindow::Impl::signUp(EObjectType objType, void * obj, void *data, chtype key) {
    auto* userData = (UserData*)data;

    auto coordinator = userData->coordinator;
    auto err = coordinator.lock()->signUp(userDataToUser(*userData));
    if (!err) {
        userData->_impl->_callback = [coordinator]() {
            coordinator.lock()->toSignIn();
        };
        return (FALSE);
    }


    const char *mesg[2];
    char msg[100];
    strcpy(msg, err->getErrorMessage().c_str());
    mesg[0] = msg;
    mesg[1] = "<C>Press any key to continue.";
    popupLabel (userData->cdk_screen, (CDK_CSTRING2) mesg, 2);
    activateCDKButton((CDKBUTTON*) obj, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::SignUpWindow::Impl::toSignIn(EObjectType objType, void *obj, void *data, chtype key) {
    auto* userData = (UserData*)data;

    auto coordinator = userData->coordinator;
    userData->_impl->_callback = [coordinator](){
        coordinator.lock()->toSignIn();
    };
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

polytour::transport::User
polytour::ui::cdk::SignUpWindow::Impl::userDataToUser(const polytour::ui::cdk::SignUpWindow::Impl::UserData& userdata) {
    polytour::transport::User user;
    user.nickname = userdata.nickname;
    user.password = userdata.password;
    user.name = userdata.name;
    user.email = userdata.email;
    user.surname = userdata.surname;
    std::string strAge = userdata.age;
    if (!strAge.empty())
        user.age = std::stoi(strAge);
    return user;
}

void polytour::ui::cdk::SignUpWindow::Impl::init() {
    cdkScreen = initCDKScreen(nullptr);
    initCDKColor ();

    nickEntry = createEntry(CENTER, "<C> Mandatory ", "</U/6>Nickname: <!U!6>");
    setCDKEntryULChar(nickEntry, ACS_ULCORNER);
    setCDKEntryURChar(nickEntry, ACS_URCORNER);

    passwordEntry = createEntry(yNextTo(nickEntry), "", "</U/6>Password: <!U!6>");
    passwordEntry->boxWidth = nickEntry->boxWidth;

    nameEntry = createEntry(yNextTo(passwordEntry), "", "</U/6>Name:     <!U!6>");
    nameEntry->boxWidth = passwordEntry->boxWidth;

    emailEntry = createEntry(yNextTo(nameEntry), "", "</U/6>Email:    <!U!6>");
    emailEntry->boxWidth = nameEntry->boxWidth;

    surnameEntry = createEntry(yNextTo(emailEntry), "<C> Not mandatory ", "</U/6>Surname:  <!U!6>");
    surnameEntry->boxWidth = emailEntry->boxWidth;

    ageEntry = createEntry(yNextTo(surnameEntry), "", "</U/6>Age:      <!U!6>");
    ageEntry->boxWidth = surnameEntry->boxWidth;

    // Init logo
    const char* name_text[] = {"              </B> Sign up  <!B>              "};
    windowName = newCDKLabel(cdkScreen, CENTER, getbegy(nickEntry->win) - nickEntry->boxHeight / 2, (CDK_CSTRING2)name_text, 1, TRUE, FALSE);

    // Init logo
    const char* label_text[] = {"               </B/K/5> Polytour <!B!K!5>                "};
    logo = newCDKLabel(cdkScreen, CENTER, yPrevFrom(nickEntry) - windowName->boxHeight, (CDK_CSTRING2)label_text, 1, TRUE, TRUE);

    // Init sing up button
    auto width = nickEntry->boxWidth;
    auto entryXPos = getbegx(nickEntry->win);
    std::string nameSingUpButton = "<C>" + std::string(width / 4 - 4, ' ') + "Sign up" + std::string(width / 4 - 4, ' ');
    signUpButton = newCDKButton(cdkScreen, entryXPos, yNextTo(ageEntry), nameSingUpButton.data(), nullptr, TRUE, FALSE);
    setCDKButtonboxULChar (signUpButton, ACS_LTEE);
    setCDKButtonboxURChar (signUpButton, ACS_TTEE);
    setCDKButtonboxLRChar (signUpButton, ACS_LRCORNER);

    // Init cancel button
    std::string name = "<C>" + std::string(width / 4 - 4, ' ') + "Cancel" + std::string(width / 4 - 4, ' ');
    cancelButton = newCDKButton(cdkScreen, entryXPos + width / 2, yNextTo(ageEntry), name.data(), nullptr, TRUE, FALSE);
    setCDKButtonboxULChar (cancelButton, ACS_TTEE);
    setCDKButtonboxURChar (cancelButton, ACS_RTEE);
    setCDKButtonboxLLChar (cancelButton, ACS_LLCORNER);

    // Init sign in button
    std::string nameSingIn = "<C>" + std::string(width / 2 - 4, ' ') + "Sign In " + std::string(width / 2 - 4, ' ');
    signInButton = newCDKButton(cdkScreen, CENTER, BOTTOM, nameSingIn.data(), nullptr, TRUE, FALSE);


    // Initialization of utility struct
    _userdata =  {
            .cdk_screen = cdkScreen,
            .coordinator = _pCoordinator,
            .nickname = nickEntry->info,
            .password = passwordEntry->info,
            .name = nameEntry->info,
            .email = emailEntry->info,
            .surname = surnameEntry->info,
            .age = ageEntry->info
    };
    _userdata._impl = this;

    // Bind section

    bindCDKObject (vENTRY, nickEntry, KEY_TAB, activateEntry, passwordEntry);
    bindCDKObject (vENTRY, nickEntry, KEY_ENTER, activateEntry, passwordEntry);
    bindCDKObject (vENTRY, passwordEntry, KEY_TAB, activateEntry, nameEntry);
    bindCDKObject (vENTRY, passwordEntry, KEY_ENTER, activateEntry, nameEntry);
    bindCDKObject (vENTRY, nameEntry, KEY_TAB, activateEntry, emailEntry);
    bindCDKObject (vENTRY, nameEntry, KEY_ENTER, activateEntry, emailEntry);
    bindCDKObject (vENTRY, emailEntry, KEY_TAB, activateEntry, surnameEntry);
    bindCDKObject (vENTRY, emailEntry, KEY_ENTER, activateEntry, surnameEntry);
    bindCDKObject (vENTRY, surnameEntry, KEY_TAB, activateEntry, ageEntry);
    bindCDKObject (vENTRY, surnameEntry, KEY_ENTER, activateEntry, ageEntry);
    bindCDKObject (vENTRY, ageEntry, KEY_TAB, activateButton, signUpButton);
    bindCDKObject (vENTRY, ageEntry, KEY_ENTER, activateButton, signUpButton);
    bindCDKObject (vBUTTON, signUpButton, KEY_TAB, activateButton, cancelButton);
    bindCDKObject (vBUTTON, cancelButton, KEY_TAB, activateButton, signInButton);
    bindCDKObject (vBUTTON, signInButton, KEY_TAB, activateEntry, nickEntry);
    bindCDKObject (vBUTTON, signInButton, KEY_ENTER, toSignIn, &_userdata);
    bindCDKObject (vBUTTON, signUpButton, KEY_ENTER, signUp, &_userdata);
    bindCDKObject (vBUTTON, cancelButton, KEY_ENTER, cancel, &_userdata);

    refreshCDKScreen (cdkScreen);
    (void)activateCDKEntry(nickEntry, nullptr);

    destroy();
    _callback();
}


polytour::ui::cdk::SignUpWindow::SignUpWindow(const std::shared_ptr<ICoordinator>& coordinator):
_pImpl(std::make_unique<Impl>(coordinator)){}

polytour::ui::cdk::SignUpWindow::~SignUpWindow() = default;

void polytour::ui::cdk::SignUpWindow::destroy() {
    _pImpl->destroy();
}

void polytour::ui::cdk::SignUpWindow::init() {
    _pImpl->init();
}
