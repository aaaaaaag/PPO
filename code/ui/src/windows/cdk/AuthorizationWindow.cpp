//
// Created by denis on 02.08.2022.
//

#include "windows/cdk/AuthorizationWindow.h"
#include "cdk/cdk.h"
#include <string>
#include <functional>
#include <utility>

class polytour::ui::cdk::AuthorizationWindow::Impl {
public:

    typedef struct UserData {
        SScreen* cdk_screen;
        std::weak_ptr<polytour::ui::ICoordinator> coordinator;
        const char* nickname;
        const char* password;
        Impl* _impl;
    } UserData;

    explicit Impl(const std::shared_ptr<ICoordinator>& coordinator): _pCoordinator(coordinator) {}

    ~Impl() {
        destroy();
    }

    void destroy() {
        if (isDestroyed)
            return;
        /* Clean up and exit. */

        destroyCDKButton(cancelButton);
        destroyCDKButton(authorizationButton);
        destroyCDKButton(signUpButton);
        destroyCDKLabel(logo);
        destroyCDKEntry (pass_entry);
        destroyCDKEntry (nick_entry);
        destroyCDKScreen (cdkScreen);
        endCDK ();
        isDestroyed = true;
    };

    void init();

    bool isDestroyed = false;
    std::weak_ptr<ICoordinator> _pCoordinator;
    SEntry* nick_entry;
    SEntry* pass_entry;
    SButton* authorizationButton;
    SButton* cancelButton;
    SButton* signUpButton;
    SLabel* logo;
    SScreen* cdkScreen;

    std::function<void()> _callback;

    UserData _userdata;

    static BINDFN_PROTO (activateEntry);
    static BINDFN_PROTO (activateButton);
    static BINDFN_PROTO (authorize);
    static BINDFN_PROTO (cancel);
    static BINDFN_PROTO (toSignUp);

};


void polytour::ui::cdk::AuthorizationWindow::Impl::init() {
    cdkScreen = initCDKScreen(nullptr);
    initCDKColor ();

    // Init nickname entry
    nick_entry = newCDKEntry (cdkScreen, CENTER, CENTER, "", "</U/6>Nickname: <!U!6>", A_NORMAL, '_', vMIXED,
                              40, 256, 256, TRUE, FALSE);
    setCDKEntryLLChar(nick_entry, ACS_LTEE);
    setCDKEntryLRChar(nick_entry, ACS_RTEE);

    // Init password entry
    pass_entry = newCDKEntry (cdkScreen, getbegx(nick_entry->win),
                              getbegy(nick_entry->win) + nick_entry->boxHeight - 1,
                              "", "</U/6>Password: <!U!6>", A_NORMAL, '_', vMIXED, 40, 0, 256, TRUE, FALSE);
    setCDKEntryULChar(pass_entry, ACS_LTEE);
    setCDKEntryURChar(pass_entry, ACS_RTEE);
    setCDKEntryLLChar(pass_entry, ACS_LTEE);
    setCDKEntryLRChar(pass_entry, ACS_RTEE);

    // Init authorization button
    auto width = nick_entry->boxWidth;
    auto entryXPos = getbegx(nick_entry->win);
    std::string nameAuthButton = "<C>" + std::string(width / 4 - 4, ' ') + "Log in" + std::string(width / 4 - 3, ' ');
    authorizationButton = newCDKButton(cdkScreen, entryXPos,
                                       getbegy (pass_entry->win) + pass_entry->boxHeight - 1,
                                       nameAuthButton.data(), nullptr, TRUE, FALSE);
    setCDKButtonboxULChar (authorizationButton, ACS_LTEE);
    setCDKButtonboxURChar (authorizationButton, ACS_TTEE);
    setCDKButtonboxLRChar (authorizationButton, ACS_LRCORNER);

    // Init cancel button
    std::string name = "<C>" + std::string(width / 4 - 4, ' ') + "Cancel" + std::string(width / 4 - 4, ' ');
    cancelButton = newCDKButton(cdkScreen, entryXPos + width / 2,
                                getbegy (pass_entry->win) + pass_entry->boxHeight - 1,
                                name.data(), nullptr, TRUE, FALSE);
    setCDKButtonboxULChar (cancelButton, ACS_TTEE);
    setCDKButtonboxURChar (cancelButton, ACS_RTEE);
    setCDKButtonboxLLChar (cancelButton, ACS_LLCORNER);

    // Init sign up button
    std::string signUpName = "<C>" + std::string(width / 2 - 4, ' ') + "Sign up " + std::string(width / 2 - 4, ' ');
    signUpButton = newCDKButton(cdkScreen, CENTER, BOTTOM, signUpName.data(), nullptr, TRUE, FALSE);

    // Init logo
    const char* label_text[] = {"               </B/K/5> Polytour <!B!K!5>                "};
    logo = newCDKLabel(cdkScreen, CENTER,
                       getbegy(nick_entry->win) - nick_entry->boxHeight - 1,
                       (CDK_CSTRING2)label_text, 1, TRUE, TRUE);

    // Initialization of utility struct
    _userdata.cdk_screen = cdkScreen;
    _userdata.coordinator = _pCoordinator;
    _userdata.nickname = nick_entry->info;
    _userdata.password = pass_entry->info;
    _userdata._impl = this;

    // Widgets binding section
    bindCDKObject (vENTRY, nick_entry, KEY_TAB, activateEntry, pass_entry);
    bindCDKObject (vENTRY, nick_entry, KEY_ENTER, activateEntry, pass_entry);
    bindCDKObject (vENTRY, pass_entry, KEY_TAB, activateButton, authorizationButton);
    bindCDKObject (vENTRY, pass_entry, KEY_ENTER, activateButton, authorizationButton);
    bindCDKObject (vBUTTON, authorizationButton, KEY_TAB, activateButton, cancelButton);
    bindCDKObject (vBUTTON, cancelButton, KEY_TAB, activateButton, signUpButton);
    bindCDKObject (vBUTTON, signUpButton, KEY_TAB, activateEntry, nick_entry);
    bindCDKObject (vBUTTON, cancelButton, KEY_ENTER, cancel, &_userdata);
    bindCDKObject(vBUTTON, authorizationButton, KEY_ENTER, authorize, &_userdata);
    bindCDKObject(vBUTTON, signUpButton, KEY_ENTER, toSignUp, &_userdata);

    refreshCDKScreen (cdkScreen);
    (void)activateCDKEntry (nick_entry, nullptr);
    destroy();
    _callback();
}

polytour::ui::cdk::AuthorizationWindow::AuthorizationWindow(const std::shared_ptr<ICoordinator>& coordinator):
_pImpl(std::make_unique<Impl>(coordinator)){
}

void polytour::ui::cdk::AuthorizationWindow::destroy() {
    _pImpl->destroy();
}

void polytour::ui::cdk::AuthorizationWindow::init() {
    _pImpl->init();
}

polytour::ui::cdk::AuthorizationWindow::~AuthorizationWindow() = default;

static int deactivateObj(EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED) {
    chtype esc = KEY_ESC;
    if (cdktype == vBUTTON)
        (void) activateCDKButton((CDKBUTTON*)object, &esc);
    else if (cdktype == vENTRY)
        (void) activateCDKEntry((CDKENTRY*) object, &esc);
    else return (FALSE);
    return (TRUE);
}

int polytour::ui::cdk::AuthorizationWindow::Impl::activateEntry(
        EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED, void *clientData, chtype key) {
    (void) activateCDKEntry((CDKENTRY *)clientData, nullptr);
    if (!deactivateObj(cdktype, object)) return (FALSE);
    return (TRUE);
}

int polytour::ui::cdk::AuthorizationWindow::Impl::activateButton(
        EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED, void *clientData, chtype key) {
    (void)activateCDKButton((CDKBUTTON *)clientData, nullptr);
    if (!deactivateObj(cdktype, object)) return (FALSE);
    return (TRUE);
}

int polytour::ui::cdk::AuthorizationWindow::Impl::cancel(
        EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED, void *clientData, chtype key)
{
    auto* userData = (UserData*)clientData;
    if (!deactivateObj(cdktype, object)) return (FALSE);
    userData->_impl->_callback = [](){

    };
    return (TRUE);
}

int polytour::ui::cdk::AuthorizationWindow::Impl::authorize (
        EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED, void *clientData, chtype key)
{
    auto* userData = (UserData*)clientData;

    auto coordinator = userData->coordinator;
    auto err = coordinator.lock()->authorize(userData->nickname, userData->password);
    if (!deactivateObj(cdktype, object)) return (FALSE);
    if (!err) {
        userData->_impl->_callback = [coordinator](){
            coordinator.lock()->toMainMenu();
        };
        return (FALSE);
    }

    const char *mesg[2];
    mesg[0] = "<C> Failed authorization ";
    mesg[1] = "<C>Press any key to continue.";
    popupLabel (userData->cdk_screen, (CDK_CSTRING2) mesg, 2);
    activateCDKButton((CDKBUTTON*)object, nullptr);
    return (FALSE);
}

int polytour::ui::cdk::AuthorizationWindow::Impl::toSignUp(
        EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED, void *clientData, chtype key)
{
    auto* userData = (UserData*)clientData;
    auto coordinator = userData->coordinator;
    userData->_impl->_callback = [coordinator]() {
        coordinator.lock()->toSignUp();
    };
    return (FALSE);
}
