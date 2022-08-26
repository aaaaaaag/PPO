//
// Created by denis on 08.08.2022.
//

#include "windows/cdk/UpdateUserWindow.h"
#include "cdk/cdk.h"
#include "AuthUserSingleton.h"
#include "User.h"
#include <cstring>

class polytour::ui::cdk::UpdateUserWindow::Impl {
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
    static void setFillField(SEntry* entry, const std::string& field);

    std::weak_ptr<ICoordinator> _pCoordinator;

    SScreen* cdkScreen;
    SEntry* nickEntry;
    SEntry* passwordEntry;
    SEntry* nameEntry;
    SEntry* emailEntry;
    SEntry* surnameEntry;
    SEntry* ageEntry;

    SLabel* logo;
    SLabel* windowName;

    SButton* backButton;
    SButton* saveButton;

    bool isDestroyed = false;

    UserData _userdata;
    std::function<void()> _callback;

    static BINDFN_PROTO (activateEntry);
    static BINDFN_PROTO (activateButton);
    static BINDFN_PROTO (save);
    static BINDFN_PROTO (back);
};

polytour::ui::cdk::UpdateUserWindow::Impl::Impl(const std::shared_ptr<ICoordinator> &coordinator):
        _pCoordinator(coordinator) {}

SEntry *polytour::ui::cdk::UpdateUserWindow::Impl::createEntry(int y, const char* label, const char* name) {
    auto result = newCDKEntry (cdkScreen, CENTER, y, label, name, A_NORMAL, '_', vMIXED,
                               40, 256, 256, TRUE, FALSE);
    setCDKEntryULChar(result, ACS_LTEE);
    setCDKEntryURChar(result, ACS_RTEE);
    setCDKEntryLLChar(result, ACS_LTEE);
    setCDKEntryLRChar(result, ACS_RTEE);
}

int polytour::ui::cdk::UpdateUserWindow::Impl::yNextTo(SEntry* entry) {
    return getbegy(entry->win) + entry->boxHeight - 1;
}

int polytour::ui::cdk::UpdateUserWindow::Impl::yPrevFrom(SEntry *entry) {
    return getbegy(entry->win) - entry->boxHeight - 1;
}

void polytour::ui::cdk::UpdateUserWindow::Impl::destroy() {
    if (isDestroyed)
        return;

    destroyCDKButton(backButton);
    destroyCDKButton(saveButton);

    destroyCDKEntry(ageEntry);
    destroyCDKEntry(surnameEntry);
    destroyCDKEntry(emailEntry);
    destroyCDKEntry(nameEntry);
    destroyCDKEntry(passwordEntry);
    destroyCDKEntry(nickEntry);

    destroyCDKLabel(logo);
    destroyCDKLabel(windowName);


    destroyCDKScreen (cdkScreen);
    endCDK ();
    isDestroyed = true;
}

int polytour::ui::cdk::UpdateUserWindow::Impl::activateEntry(EObjectType objType, void * obj, void * clientData, chtype key) {
    if (!deactivateObj(objType, obj)) return (FALSE);

    auto *entry = (CDKENTRY *)clientData;
    (void) activateCDKEntry(entry, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::UpdateUserWindow::Impl::activateButton(EObjectType objType, void * obj, void * clientData, chtype key) {
    if (!deactivateObj(objType, obj)) return (FALSE);

    auto *buttonbox = (CDKBUTTON *)clientData;
    (void)activateCDKButton(buttonbox, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::UpdateUserWindow::Impl::deactivateObj(EObjectType cdktype, void *object) {
    chtype esc = KEY_ESC;
    if (cdktype == vBUTTON)
        (void) activateCDKButton((CDKBUTTON*)object, &esc);
    else if (cdktype == vENTRY)
        (void) activateCDKEntry((CDKENTRY*) object, &esc);
    else return (FALSE);
    return (TRUE);
}

int polytour::ui::cdk::UpdateUserWindow::Impl::back(EObjectType objType, void * obj, void *data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->coordinator;
    userData->_impl->_callback = [coordinator](){
        coordinator.lock()->toMainMenu();
    };
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

// TODO rework
int polytour::ui::cdk::UpdateUserWindow::Impl::save(EObjectType objType, void * obj, void *data, chtype key) {
    auto* userData = (UserData*)data;

    auto coordinator = userData->coordinator;
    coordinator.lock()->getMainAPI().userAPI()->updateUser(userDataToUser(*userData));
    if (!coordinator.lock()->getMainAPI().userAPI()->isError()) {
        userData->_impl->_pCoordinator.lock()->getMainAPI().userAPI()->auth(userData->nickname, userData->password);
        const char *mesg[2];
        mesg[0] = "<C> Success ";
        mesg[1] = "<C>Press any key to continue.";
        popupLabel (userData->cdk_screen, (CDK_CSTRING2) mesg, 2);
        //activateCDKButton((CDKBUTTON*)obj, nullptr);
        return (TRUE);
    }

    auto err = coordinator.lock()->getMainAPI().userAPI()->getError();
    const char *mesg[2];
    char msg[100];
    strcpy(msg, err->getErrorMessage().c_str());
    mesg[0] = msg;
    mesg[1] = "<C>Press any key to continue.";
    popupLabel (userData->cdk_screen, (CDK_CSTRING2) mesg, 2);
    return (FALSE);
}

polytour::transport::User
polytour::ui::cdk::UpdateUserWindow::Impl::userDataToUser(const polytour::ui::cdk::UpdateUserWindow::Impl::UserData& userdata) {
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

void polytour::ui::cdk::UpdateUserWindow::Impl::init() {
    cdkScreen = initCDKScreen(nullptr);
    initCDKColor ();

    auto user = _pCoordinator.lock()->getMainAPI().userAPI()->currentUser();

    nickEntry = createEntry(CENTER, "<C> Mandatory ", "</U/6>Nickname: <!U!6>");
    setFillField(nickEntry, user.nickname);
    setCDKEntryULChar(nickEntry, ACS_ULCORNER);
    setCDKEntryURChar(nickEntry, ACS_URCORNER);

    passwordEntry = createEntry(yNextTo(nickEntry), "", "</U/6>Password: <!U!6>");
    setFillField(passwordEntry, user.password);
    passwordEntry->boxWidth = nickEntry->boxWidth;

    nameEntry = createEntry(yNextTo(passwordEntry), "", "</U/6>Name:     <!U!6>");
    setFillField(nameEntry, user.name);
    nameEntry->boxWidth = passwordEntry->boxWidth;

    emailEntry = createEntry(yNextTo(nameEntry), "", "</U/6>Email:    <!U!6>");
    setFillField(emailEntry, user.email);
    emailEntry->boxWidth = nameEntry->boxWidth;

    surnameEntry = createEntry(yNextTo(emailEntry), "<C> Not mandatory ", "</U/6>Surname:  <!U!6>");
    if (user.surname.hasValue())
        setFillField(surnameEntry, user.surname.getValue());
    surnameEntry->boxWidth = emailEntry->boxWidth;

    ageEntry = createEntry(yNextTo(surnameEntry), "", "</U/6>Age:      <!U!6>");
    if (user.age.hasValue()) setFillField(ageEntry, std::to_string(user.age.getValue()));
    ageEntry->boxWidth = surnameEntry->boxWidth;

    // Init logo
    const char* name_text[] = {"              </B> User info  <!B>              "};
    windowName = newCDKLabel(cdkScreen, CENTER, getbegy(nickEntry->win) - nickEntry->boxHeight / 2, (CDK_CSTRING2)name_text, 1, TRUE, FALSE);

    // Init logo
    const char* label_text[] = {"               </B/K/5> Polytour <!B!K!5>                "};
    logo = newCDKLabel(cdkScreen, CENTER, yPrevFrom(nickEntry) - windowName->boxHeight, (CDK_CSTRING2)label_text, 1, TRUE, TRUE);

    // Init sing up button
    auto width = nickEntry->boxWidth;
    auto entryXPos = getbegx(nickEntry->win);
    std::string nameSingUpButton = "<C>" + std::string(width / 4 - 3, ' ') + " Save " + std::string(width / 4 - 4, ' ');
    saveButton = newCDKButton(cdkScreen, entryXPos, yNextTo(ageEntry), nameSingUpButton.data(), nullptr, TRUE, FALSE);
    setCDKButtonboxULChar (saveButton, ACS_LTEE);
    setCDKButtonboxURChar (saveButton, ACS_TTEE);
    setCDKButtonboxLRChar (saveButton, ACS_LRCORNER);

    // Init cancel button
    std::string name = "<C>" + std::string(width / 4 - 4, ' ') + " Back " + std::string(width / 4 - 4, ' ');
    backButton = newCDKButton(cdkScreen, entryXPos + width / 2, yNextTo(ageEntry), name.data(), nullptr, TRUE, FALSE);
    setCDKButtonboxULChar (backButton, ACS_TTEE);
    setCDKButtonboxURChar (backButton, ACS_RTEE);
    setCDKButtonboxLLChar (backButton, ACS_LLCORNER);


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
    bindCDKObject (vENTRY, ageEntry, KEY_TAB, activateButton, saveButton);
    bindCDKObject (vENTRY, ageEntry, KEY_ENTER, activateButton, saveButton);
    bindCDKObject (vBUTTON, saveButton, KEY_TAB, activateButton, backButton);
    bindCDKObject (vBUTTON, backButton, KEY_TAB, activateEntry, nickEntry);
    bindCDKObject (vBUTTON, saveButton, KEY_ENTER, save, &_userdata);
    bindCDKObject (vBUTTON, backButton, KEY_ENTER, back, &_userdata);

    refreshCDKScreen (cdkScreen);
    (void)activateCDKEntry(nickEntry, nullptr);

    destroy();
    _callback();
}

void polytour::ui::cdk::UpdateUserWindow::Impl::setFillField(SEntry *entry, const std::string& field) {
    for (const char& el: field) { injectCDKEntry(entry, el); }
}

void polytour::ui::cdk::UpdateUserWindow::destroy() {
    _pImpl->destroy();
}

void polytour::ui::cdk::UpdateUserWindow::init() {
    _pImpl->init();
}

polytour::ui::cdk::UpdateUserWindow::UpdateUserWindow(const std::shared_ptr<ICoordinator>& coordinator):
_pImpl(std::make_unique<Impl>(coordinator)){}

polytour::ui::cdk::UpdateUserWindow::~UpdateUserWindow() = default;
