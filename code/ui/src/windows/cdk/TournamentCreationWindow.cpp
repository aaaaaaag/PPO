//
// Created by denis on 09.08.2022.
//

#include <cmath>
#include "windows/cdk/TournamentCreationWindow.h"
#include "cdk/cdk.h"
#include "Tournament.h"

class polytour::ui::cdk::TournamentCreationWindow::Impl {
public:

    explicit Impl(const std::shared_ptr<ICoordinator>& coordinator);
    ~Impl();

    void init();
    void destroy();

    typedef struct UserData {
        SScreen* cdk_screen;
        std::weak_ptr<polytour::ui::ICoordinator> coordinator;
        char* name;
        char* description;
        SItemList* participants;
        Impl* _impl;
    } UserData;

private:
    std::weak_ptr<ICoordinator> _pCoordinator;

    bool _isDestroyed = false;

    SScreen* _cdkScreen;
    SLabel* _logo;
    SLabel* _windowName;
    SEntry* _tourName;
    SMentry* _tourDescription;
    SItemList* _tourParticipants;

    SButton* _cancelButton;
    SButton* _createButton;

    std::function<void()> _callback;
    UserData _userdata;

    static int deactivateObj(EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED);

    static BINDFN_PROTO (activateEntry);
    static BINDFN_PROTO (activateButton);
    static BINDFN_PROTO (activateMEntry);
    static BINDFN_PROTO (activateItemList);

    static BINDFN_PROTO (create);
    static BINDFN_PROTO (cancel);
};

polytour::ui::cdk::TournamentCreationWindow::Impl::Impl(const std::shared_ptr<ICoordinator>& coordinator):
_pCoordinator(coordinator){}

polytour::ui::cdk::TournamentCreationWindow::Impl::~Impl() {
    destroy();
}

void polytour::ui::cdk::TournamentCreationWindow::Impl::init() {

    const char* tournamentParticipants[5];
    tournamentParticipants[0] = "2";
    tournamentParticipants[1] = "4";
    tournamentParticipants[2] = "8";
    tournamentParticipants[3] = "16";
    tournamentParticipants[4] = "32";

    _cdkScreen = initCDKScreen(nullptr);
    initCDKColor ();

    _tourName = newCDKEntry(_cdkScreen, CENTER, CENTER, "", "Name: ", A_NORMAL, '_', vMIXED, 47, 256, 256, TRUE, FALSE);
    setCDKEntryLLChar(_tourName, ACS_LTEE);
    setCDKEntryLRChar(_tourName, ACS_RTEE);

    _tourDescription = newCDKMentry(_cdkScreen, CENTER, getbegy(_tourName->win) + _tourName->boxHeight - 1, "",
                                    "Description: ", A_NORMAL, '_', vMIXED, 40, 5, 5, 1, TRUE, FALSE);
    setCDKMentryULChar(_tourDescription, ACS_LTEE);
    setCDKMentryURChar(_tourDescription, ACS_RTEE);
    setCDKMentryLLChar(_tourDescription, ACS_LTEE);
    setCDKMentryLRChar(_tourDescription, ACS_RTEE);

    _tourParticipants = newCDKItemlist(_cdkScreen, CENTER, getbegy(_tourDescription->win) + _tourDescription->boxHeight - 1,
                                       "", "Participants:                                     ",
                                       (CDK_CSTRING2)tournamentParticipants, 5, 0, TRUE, FALSE);
    setCDKItemlistULChar(_tourParticipants, ACS_LTEE);
    setCDKItemlistURChar(_tourParticipants, ACS_RTEE);
    setCDKItemlistLLChar(_tourParticipants, ACS_LTEE);
    setCDKItemlistLRChar(_tourParticipants, ACS_RTEE);

    chtype keyEsc = KEY_ESC;
    activateCDKItemlist(_tourParticipants, &keyEsc);
    const char* winName[] = {"               </B/4> Tournament info <!B!4>                "};
    _windowName = newCDKLabel(_cdkScreen, CENTER, getbegy(_tourName->win) - _tourName->boxHeight + 1, (CDK_CSTRING2)winName, 1, TRUE, FALSE);
    const char* label_text[] = {"               </B/K/5> Polytour <!B!K!5>                "};
    _logo = newCDKLabel(_cdkScreen, CENTER, getbegy(_windowName->win) - _windowName->boxHeight , (CDK_CSTRING2)label_text, 1, TRUE, FALSE);
    auto width = _tourName->boxWidth;

    // Init create button
    std::string nameCreate = "<C>" + std::string(width / 4 - 3, ' ') + "Create" + std::string(width / 4 - 3, ' ');
    _createButton = newCDKButton(_cdkScreen, getbegx(_tourName->win),
                                 getbegy(_tourParticipants->win) + _tourParticipants->boxHeight - 1,
                                 nameCreate.data(), nullptr, TRUE, FALSE);
    setCDKButtonboxULChar (_createButton, ACS_LTEE);
    setCDKButtonboxURChar (_createButton, ACS_TTEE);
    setCDKButtonboxLRChar (_createButton, ACS_LRCORNER);

    // Init cancel button
    std::string name = "<C>" + std::string(width / 4 - 3, ' ') + "Cancel" + std::string(width / 4 - 3, ' ');
    _cancelButton = newCDKButton(_cdkScreen, getbegx(_tourName->win) + width / 2,
                                 getbegy(_tourParticipants->win) + _tourParticipants->boxHeight - 1,
                                 name.data(), nullptr, TRUE, FALSE);
    setCDKButtonboxULChar (_cancelButton, ACS_TTEE);
    setCDKButtonboxURChar (_cancelButton, ACS_RTEE);
    setCDKButtonboxLLChar (_cancelButton, ACS_LLCORNER);

    _userdata = UserData{
        .cdk_screen = _cdkScreen,
        .coordinator = _pCoordinator,
        .name = _tourName->info,
        .description = _tourDescription->info,
        .participants = _tourParticipants
    };
    _userdata._impl = this;

    // Widgets binding section
    bindCDKObject (vENTRY, _tourName, KEY_TAB, activateMEntry, _tourDescription);
    bindCDKObject (vENTRY, _tourName, KEY_ENTER, activateMEntry, _tourDescription);
    bindCDKObject (vMENTRY, _tourDescription, KEY_TAB, activateItemList, _tourParticipants);
    bindCDKObject (vMENTRY, _tourDescription, KEY_ENTER, activateItemList, _tourParticipants);
    bindCDKObject (vITEMLIST, _tourParticipants, KEY_TAB, activateButton, _createButton);
    bindCDKObject (vITEMLIST, _tourParticipants, KEY_ENTER, activateButton, _createButton);
    bindCDKObject (vBUTTON, _createButton, KEY_TAB, activateButton, _cancelButton);
    bindCDKObject (vBUTTON, _cancelButton, KEY_TAB, activateEntry, _tourName);
    bindCDKObject (vBUTTON, _cancelButton, KEY_ENTER, cancel, &_userdata);
    bindCDKObject(vBUTTON, _createButton, KEY_ENTER, create, &_userdata);


    refreshCDKScreen(_cdkScreen);
    activateCDKEntry(_tourName, nullptr);

    destroy();
    _callback();
}

int polytour::ui::cdk::TournamentCreationWindow::Impl::deactivateObj(EObjectType cdktype, void *object) {
    chtype esc = KEY_ESC;
    if (cdktype == vBUTTON)
        (void) activateCDKButton((CDKBUTTON*)object, &esc);
    else if (cdktype == vENTRY)
        (void) activateCDKEntry((CDKENTRY*) object, &esc);
    else if (cdktype == vMENTRY)
        (void) activateCDKMentry((CDKMENTRY*) object, &esc);
    else if (cdktype == vITEMLIST)
        (void) activateCDKItemlist((CDKITEMLIST*) object, &esc);
    else
        return (FALSE);
    return (TRUE);
}

int polytour::ui::cdk::TournamentCreationWindow::Impl::activateEntry(EObjectType objType, void * obj, void * data, chtype key) {
    if (!deactivateObj(objType, obj)) return (FALSE);
    (void) activateCDKEntry((CDKENTRY *)data, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::TournamentCreationWindow::Impl::activateButton(EObjectType objType, void * obj, void * data, chtype key) {
    if (!deactivateObj(objType, obj)) return (FALSE);
    (void) activateCDKButton((CDKBUTTON *)data, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::TournamentCreationWindow::Impl::activateMEntry(EObjectType objType, void * obj, void * data, chtype key) {
    if (!deactivateObj(objType, obj)) return (FALSE);
    (void) activateCDKMentry((CDKMENTRY*)data, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::TournamentCreationWindow::Impl::activateItemList(EObjectType objType, void * obj, void * data, chtype key) {
    if (!deactivateObj(objType, obj)) return (FALSE);
    (void) activateCDKItemlist((CDKITEMLIST *)data, nullptr);
    return (TRUE);
}

int polytour::ui::cdk::TournamentCreationWindow::Impl::cancel(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->coordinator;
    userData->_impl->_callback = [coordinator](){
        coordinator.lock()->toMainMenu();
    };
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

int polytour::ui::cdk::TournamentCreationWindow::Impl::create(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->coordinator;

    std::string name = userData->name;
    std::string description = userData->description;
    auto participants = pow(2, userData->participants->currentItem);
    polytour::transport::Tournament tournament;
    tournament.name = userData->name;
    tournament.description = userData->description;
    tournament.cur_participants_num = 0;
    tournament.max_participants_num = pow(2, userData->participants->currentItem + 1);
    tournament.organizer_id = coordinator.lock()->getMainAPI().userAPI()->currentUser().id;
    tournament.status = polytour::transport::Tournament::status_wait_for_participants();
    coordinator.lock()->getMainAPI().tournamentAPI()->create(tournament);
    if (!coordinator.lock()->getMainAPI().tournamentAPI()->isError()) {
        userData->_impl->_callback = [coordinator](){
            coordinator.lock()->toMainMenu();
        };
        return (FALSE);
    }

    auto err = coordinator.lock()->getMainAPI().tournamentAPI()->getError();
    const char *mesg[2];
    char msg[100];
    strcpy(msg, err->getErrorMessage().c_str());
    mesg[0] = msg;
    mesg[1] = "<C>Press any key to continue.";
    popupLabel (userData->cdk_screen, (CDK_CSTRING2) mesg, 2);
    return (TRUE);
}

void polytour::ui::cdk::TournamentCreationWindow::Impl::destroy() {
    if (_isDestroyed)
        return;

    destroyCDKEntry(_tourName);
    destroyCDKMentry(_tourDescription);
    destroyCDKItemlist(_tourParticipants);

    destroyCDKLabel(_windowName);
    destroyCDKLabel(_logo);
    destroyCDKScreen (_cdkScreen);
    endCDK ();
    _isDestroyed = true;
}

polytour::ui::cdk::TournamentCreationWindow::TournamentCreationWindow(
        const std::shared_ptr<ICoordinator> &coordinator):
        _pImpl(std::make_unique<Impl>(coordinator)){}

polytour::ui::cdk::TournamentCreationWindow::~TournamentCreationWindow() = default;

void polytour::ui::cdk::TournamentCreationWindow::destroy() {
    _pImpl->destroy();
}

void polytour::ui::cdk::TournamentCreationWindow::init() {
    _pImpl->init();
}
