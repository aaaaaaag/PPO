//
// Created by denis on 04.08.2022.
//

#include "windows/cdk/MainMenuWindow.h"
#include <cdk/cdk.h>
#include <utility>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdarg>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
#include "Tournament.h"


template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

class polytour::ui::MainMenuWindow::Impl {
public:

    explicit Impl(const std::shared_ptr<ICoordinator>& coordinator);

    void destroy();

    void init();

    ~Impl() {
        destroy();
    }

    typedef struct UserData {
        SScreen* cdk_screen;
        std::weak_ptr<polytour::ui::ICoordinator> coordinator;
        Impl* _impl;
    } UserData;

    static int deactivateObj(EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED);

private:

    bool _isDestroyed = false;

    void initTournamentList();
    std::vector<char*> getSelectionListElems();

    std::weak_ptr<ICoordinator> _pCoordinator;
    std::vector<transport::Tournament> _vActiveTournaments;

    SScreen *cdkScreen;
    SScroll* tourSelection{};
    SButton* _logOutButton;
    SButton* _quitButton;
    SButton* _createTournamentButton;
    SButton* _editProfileButton;

    std::function<void()> _callback;
    UserData _userdata;

    static BINDFN_PROTO (activateScroll);
    static BINDFN_PROTO (activateLogOut);
    static BINDFN_PROTO (activateQuit);
    static BINDFN_PROTO (activateUpdateUser);
    static BINDFN_PROTO (activateCreateTournament);
    static BINDFN_PROTO (goToNextButton);
    static BINDFN_PROTO (goToNextScroll);
    static BINDFN_PROTO (goToTournament);
};

polytour::ui::MainMenuWindow::Impl::Impl(const std::shared_ptr<ICoordinator> &coordinator):
        _pCoordinator(coordinator){}


void polytour::ui::MainMenuWindow::Impl::init() {
    cdkScreen = initCDKScreen(nullptr);
    initCDKColor ();
    initTournamentList();
    _editProfileButton = newCDKButton(cdkScreen, getbegx(tourSelection->win) + tourSelection->boxWidth, TOP,
                                            "<C>   Edit profile    ", nullptr, TRUE, FALSE);
    _createTournamentButton = newCDKButton(cdkScreen, getbegx(tourSelection->win)  + tourSelection->boxWidth,
                                           getbegy(_editProfileButton->win) + _editProfileButton->boxHeight,
                                            "<C> Create tournament ", nullptr, TRUE, FALSE);
    _logOutButton = newCDKButton(cdkScreen, getbegx(tourSelection->win)  + tourSelection->boxWidth,
                                            getbegy(_createTournamentButton->win) + _createTournamentButton->boxHeight,
                                            "<C>     Log out       ", nullptr, TRUE, FALSE);
    _quitButton = newCDKButton(cdkScreen, getbegx(tourSelection->win) + tourSelection->boxWidth,
                                          getbegy(_logOutButton->win) + _logOutButton->boxHeight,
                                            "<C>       Quit        ", nullptr, TRUE, FALSE);

    _userdata = {
            .cdk_screen = cdkScreen,
            .coordinator = _pCoordinator,
            ._impl = this
    };

    bindCDKObject (vSCROLL, tourSelection, KEY_TAB, goToNextButton, _editProfileButton);
    bindCDKObject (vBUTTON, _editProfileButton, KEY_TAB, goToNextButton, _createTournamentButton);
    bindCDKObject (vBUTTON, _createTournamentButton, KEY_TAB, goToNextButton, _logOutButton);
    bindCDKObject (vBUTTON, _logOutButton, KEY_TAB, goToNextButton, _quitButton);
    bindCDKObject (vBUTTON, _quitButton, KEY_TAB, goToNextScroll, tourSelection);

    bindCDKObject (vBUTTON, _logOutButton, KEY_ENTER, activateLogOut, &_userdata);
    bindCDKObject (vBUTTON, _quitButton, KEY_ENTER, activateQuit, &_userdata);
    bindCDKObject (vBUTTON, _editProfileButton, KEY_ENTER, activateUpdateUser, &_userdata);
    bindCDKObject(vBUTTON, _createTournamentButton, KEY_ENTER, activateCreateTournament, &_userdata);
    bindCDKObject(vSCROLL, tourSelection, KEY_ENTER, goToTournament, &_userdata);

    refreshCDKScreen(cdkScreen);
    activateCDKScroll(tourSelection, nullptr);
    destroy();
    _callback();
}

void polytour::ui::MainMenuWindow::Impl::initTournamentList() {

    int height;
    int width;
    // HAHA here is fucking shit
    {
        const char *mesg[1];
        mesg[0] = "baba";
        auto first = newCDKLabel(cdkScreen, CENTER, CENTER, (CDK_CSTRING2)mesg, 1, TRUE, FALSE);
        auto second = newCDKLabel(cdkScreen, LEFT, BOTTOM, (CDK_CSTRING2)mesg, 1, TRUE, FALSE);
        height = second->ypos - first->ypos;
        width = first->xpos - second->xpos;
        destroyCDKLabel(first);
        destroyCDKLabel(second);
    }

    auto selectionElems = getSelectionListElems();
    tourSelection = newCDKScroll(
            cdkScreen, LEFT, TOP, RIGHT, height * 2, width,
            "<C> Active tournaments\n   id |      name     | cur_users | max_users | key | status |",
            &selectionElems[0], selectionElems.size(), TRUE,
            A_REVERSE, TRUE, FALSE
            );
    //bindCDKObject(vSCROLL, tourSelection, KEY_ENTER, activateScroll, &_vActiveTournaments);
}


char *convert(const std::string & s) {
    char *pc = new char[s.size()+1];
    std::strcpy(pc, s.c_str());
    return pc;
}

std::vector<char *> polytour::ui::MainMenuWindow::Impl::getSelectionListElems() {
    std::vector<char *> result;
    std::vector<std::string> preResult;

    auto tournamentAPI = _pCoordinator.lock()->getMainAPI().tournamentAPI();
    _vActiveTournaments = tournamentAPI->getTournaments({});
    for (const auto& tournament: _vActiveTournaments) {
        std::string valueStr = string_format("|%-15s|%-11d|%-11d|%-5d|%-8s|",
                                             tournament.name.data(), tournament.cur_participants_num,
                                             tournament.max_participants_num, tournament.id, tournament.status.data());
        preResult.emplace_back(valueStr);
    }

    std::transform(preResult.begin(), preResult.end(), std::back_inserter(result), convert);
    return result;
}

int polytour::ui::MainMenuWindow::Impl::activateScroll (
        EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED, void *clientData, chtype key) {
    auto activeTournaments = (std::vector<polytour::transport::Tournament>*)clientData;
    auto scroll = (CDKSCROLL*)object;
    auto curItemId = getCDKScrollCurrentItem(scroll);
    auto t = (*activeTournaments)[curItemId];
}

void polytour::ui::MainMenuWindow::Impl::destroy() {
    if (_isDestroyed)
        return;

//    unbindCDKObject(vSCROLL, tourSelection, KEY_ENTER);
//    unbindCDKObject(vSCROLL, tourSelection, KEY_TAB);
//    unbindCDKObject(vBUTTON, _editProfileButton, KEY_ENTER);
//    unbindCDKObject(vBUTTON, _editProfileButton, KEY_TAB);
//    unbindCDKObject(vBUTTON, _createTournamentButton, KEY_ENTER);
//    unbindCDKObject(vBUTTON, _createTournamentButton, KEY_TAB);
//    unbindCDKObject(vBUTTON, _logOutButton, KEY_ENTER);
//    unbindCDKObject(vBUTTON, _logOutButton, KEY_TAB);
//    unbindCDKObject(vBUTTON, _quitButton, KEY_ENTER);
//    unbindCDKObject(vBUTTON, _quitButton, KEY_TAB);

    //deactivateObj(vBUTTON, _editProfileButton);

    //destroyCDKScroll(tourSelection);
    destroyCDKButton(_editProfileButton);
    _editProfileButton = nullptr;
    destroyCDKButton(_createTournamentButton);
    destroyCDKButton(_logOutButton);
    destroyCDKButton(_quitButton);

    destroyCDKScroll(tourSelection);
    destroyCDKScreen (cdkScreen);
    endCDK ();
    _isDestroyed = true;
}

int polytour::ui::MainMenuWindow::Impl::activateLogOut(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->coordinator;
    userData->_impl->_callback = [coordinator](){
        coordinator.lock()->toSignIn();
    };
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

int polytour::ui::MainMenuWindow::Impl::activateQuit(EObjectType objectType, void *obj, void * data, chtype) {
    auto* userData = (UserData*)data;
    userData->_impl->_callback = [](){};
    if (!deactivateObj(objectType, obj)) return (FALSE);
    return (TRUE);
}

int polytour::ui::MainMenuWindow::Impl::goToNextButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto *entry = (CDKBUTTON *)data;
    (void) activateCDKButton(entry, nullptr);
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

int polytour::ui::MainMenuWindow::Impl::goToNextScroll(EObjectType objType, void * obj, void * data, chtype key) {
    auto *entry = (CDKSCROLL *)data;
    (void) activateCDKScroll(entry, nullptr);
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

int polytour::ui::MainMenuWindow::Impl::deactivateObj(EObjectType cdktype, void *object) {
    chtype esc = KEY_ESC;
    if (cdktype == vBUTTON)
        (void) activateCDKButton((CDKBUTTON*)object, &esc);
    else if (cdktype == vENTRY)
        (void) activateCDKEntry((CDKENTRY*) object, &esc);
    else if (cdktype == vSCROLL)
        (void) activateCDKScroll((CDKSCROLL *) object, &esc);
    else return (FALSE);
    return (TRUE);
}

int polytour::ui::MainMenuWindow::Impl::activateUpdateUser(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;

    auto coordinator = userData->coordinator;
    userData->_impl->_callback = [coordinator](){
        coordinator.lock()->toUpdateUser();
    };
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

int polytour::ui::MainMenuWindow::Impl::activateCreateTournament(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->coordinator;
    userData->_impl->_callback = [coordinator](){
        coordinator.lock()->toTournamentCreation();
    };
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

int polytour::ui::MainMenuWindow::Impl::goToTournament(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;

    auto selectedTournamentId = getCDKScrollCurrentItem(userData->_impl->tourSelection);
    auto selectedTournament = userData->_impl->_vActiveTournaments[selectedTournamentId];

    auto coordinator = userData->coordinator;
    userData->_impl->_callback = [coordinator, selectedTournament](){
        coordinator.lock()->toTournament(selectedTournament);
    };
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

polytour::ui::MainMenuWindow::MainMenuWindow(const std::shared_ptr<ICoordinator>& coordinator):
_pImpl(std::make_unique<Impl>(coordinator)){
}

void polytour::ui::MainMenuWindow::destroy() {
    _pImpl->destroy();
}

void polytour::ui::MainMenuWindow::init() {
    _pImpl->init();
}

polytour::ui::MainMenuWindow::~MainMenuWindow() = default;
