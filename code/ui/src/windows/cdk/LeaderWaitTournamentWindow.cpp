//
// Created by denis on 19.08.2022.
//

#include "windows/cdk/LeaderWaitTournamentWindow.h"

#include <utility>
#include <cmath>
#include <cstring>
#include "cdk/cdk.h"

class polytour::ui::cdk::LeaderWaitTournamentWindow::Impl {
public:

    explicit Impl(const std::shared_ptr<ICoordinator>& coordinator, transport::Tournament tournament);
    ~Impl();

    void init();
    void destroy();

    static int deactivateObj(EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED);

    typedef struct UserData {
        Impl* _impl;
        CDKSCROLL* _tourMatchesList;
    } UserData;

    static char *convert(const std::string & s);

    template<typename ... Args>
    std::string string_format( const std::string& format, Args ... args );

    CDKSCROLL* createParticipantsList();
    CDKITEMLIST* createTourList();
    CDKSCROLL * createMatchesList();
    CDKBUTTON * createStartTournamentButton();
    CDKLABEL * createTournamentLabel();
    CDKBUTTON * createToMainMenuButton();
    CDKBUTTON * createToDeleteButton();
    void rebindMatches();
    void refresh();
    void popupParticipantLabel(const std::optional<transport::User>& user);


private:
    UserData _userdata;
    std::weak_ptr<ICoordinator> _pCoordinator;
    transport::Tournament _tournament;
    std::function<void()> _callback;
    std::function<void()> _finalCallback;

    bool _isQuit = false;

    CDKSCREEN* _cdkScreen;
    CDKLABEL* _logo;
    CDKLABEL* _tournamentName;
    CDKSCROLL* _participantsList;
    CDKBUTTON* _startButton;
    CDKITEMLIST* _tourList;
    CDKSCROLL* _tourMatchesList;
    CDKBUTTON* _toMainMenuButton;
    CDKBUTTON* _deleteButton;

    std::vector<transport::User> _vParticipants;
    std::vector<transport::Match> _vTourMatches;
    std::optional<transport::User> _participant1;
    std::optional<transport::User> _participant2;

    static BINDFN_PROTO (toTourSelection);
    static BINDFN_PROTO (toMatchSelection);
    static BINDFN_PROTO (toStartButton);
    static BINDFN_PROTO (toMainMenuButton);
    static BINDFN_PROTO (toDeleteButton);
    static BINDFN_PROTO (toMatchParticipants);

    static BINDFN_PROTO (increaseTour);
    static BINDFN_PROTO (decreaseTour);
    static BINDFN_PROTO (startTournament);
    static BINDFN_PROTO (toMainMenu);
    static BINDFN_PROTO (chooseUser);
    static BINDFN_PROTO (deleteTournament);
};

polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::Impl(const std::shared_ptr<ICoordinator> &coordinator,
                                                     transport::Tournament tournament):
        _pCoordinator(coordinator),
        _tournament(std::move(tournament)){
    _vParticipants = _pCoordinator.lock()->getMainAPI().tournamentAPI()->getTournamentParticipants(_tournament);
}

polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::~Impl() {
    _isQuit = true;
    destroy();
}

void polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::init() {
    _cdkScreen = initCDKScreen(nullptr);
    initCDKColor ();

    const char* label_text[] = {"               </B/K/5> Polytour <!B!K!5>                "};
    _logo = newCDKLabel(_cdkScreen, CENTER, TOP, (CDK_CSTRING2)label_text, 1, TRUE, TRUE);
    _tournamentName = createTournamentLabel();
    _participantsList = createParticipantsList();
    _tourList = createTourList();
    _tourMatchesList = createMatchesList();
    _startButton = createStartTournamentButton();
    _toMainMenuButton = createToMainMenuButton();
    _deleteButton = createToDeleteButton();

    _userdata._impl = this;
    _userdata._tourMatchesList = _tourMatchesList;

    bindCDKObject (vSCROLL, _participantsList, KEY_TAB, toTourSelection, &_userdata);
    bindCDKObject (vITEMLIST, _tourList, KEY_TAB, toMatchSelection, &_userdata);
    bindCDKObject (vSCROLL, _tourMatchesList, KEY_TAB, toStartButton, &_userdata);
    bindCDKObject (vBUTTON, _startButton, KEY_TAB, toMainMenuButton, &_userdata);
    bindCDKObject (vBUTTON, _toMainMenuButton, KEY_TAB, toDeleteButton, &_userdata);
    bindCDKObject (vBUTTON, _deleteButton, KEY_TAB, toMatchParticipants, &_userdata);

    bindCDKObject(vITEMLIST, _tourList, KEY_UP, increaseTour, &_userdata);
    bindCDKObject(vITEMLIST, _tourList, KEY_RIGHT, increaseTour, &_userdata);
    bindCDKObject(vITEMLIST, _tourList, KEY_DOWN, decreaseTour, &_userdata);
    bindCDKObject(vITEMLIST, _tourList, KEY_LEFT, decreaseTour, &_userdata);
    bindCDKObject(vBUTTON, _startButton, KEY_ENTER, startTournament, &_userdata);
    bindCDKObject(vBUTTON, _toMainMenuButton, KEY_ENTER, toMainMenu, &_userdata);
    bindCDKObject(vBUTTON, _deleteButton, KEY_ENTER, deleteTournament, &_userdata);

    refreshCDKScreen(_cdkScreen);
    activateCDKScroll(_participantsList, nullptr);

    do{
        _callback();
    } while(!_isQuit);

    destroy();
    _finalCallback();
}

void polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::destroy() {
    destroyCDKScroll(_participantsList);
    destroyCDKItemlist(_tourList);
    destroyCDKScroll(_tourMatchesList);
    destroyCDKButton(_startButton);
    destroyCDKButton(_toMainMenuButton);

    destroyCDKScreen(_cdkScreen);
    endCDK();
}

CDKSCROLL *polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::createParticipantsList() {
    int participantsListStartY = getbegy(_tournamentName->win) + _tournamentName->boxHeight + 1;

    const char* resultList[_vParticipants.size()];
    for (int i = 0; i < _vParticipants.size(); i++)
        resultList[i] = _vParticipants[i].nickname.c_str();

    const char * label = convert(string_format("Participants %d/%d", _tournament.cur_participants_num, _tournament.max_participants_num));
    auto* result = newCDKScroll(_cdkScreen, LEFT, participantsListStartY, RIGHT,
                                LINES - participantsListStartY - 10, 40, label, (CDK_CSTRING2)resultList,
                                _vParticipants.size(), TRUE, A_REVERSE, TRUE, FALSE);

    return result;
}

CDKITEMLIST *polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::createTourList() {
    int tours = (int)sqrt((double)_tournament.max_participants_num);
    std::vector<std::string> toursStr;
    for (int i = 0; i < tours; i++)
        toursStr.emplace_back(string_format("%d", i + 1));

    char* toursStrRes[tours];
    for (int i = 0; i < tours; i++) {
        toursStrRes[i] = convert(toursStr[i]);
    }
    char label[65];
    const char* tour = "Tour: ";
    sprintf(label, "%-64s", tour);
    auto* result = newCDKItemlist(_cdkScreen, getbegx(_participantsList->win) + _participantsList->boxWidth + 1,
                                  getbegy(_participantsList->win), "", label,
                                  (CDK_CSTRING2)toursStrRes, tours, 0, TRUE, FALSE);
    chtype ecs = KEY_ESC;
    activateCDKItemlist(result, &ecs);
    return result;
}

CDKSCROLL *polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::createMatchesList() {
    auto selectedTour = getCDKItemlistCurrentItem(_tourList) + 1;
    _vTourMatches = _pCoordinator.lock()->getMainAPI().matchAPI()->getMatches({
                                                                                      .tournament_id_ = _tournament.id,
                                                                                      .tour_ = selectedTour
                                                                              });

    std::string label = string_format("%-65s\n|%-24s|%-15s|%-6s|%-15s|", "Matches", "name", "status", "key", "winner");

    char* matchesStr[_vTourMatches.size()];
    for (int i = 0; i < _vTourMatches.size(); i++) {
        auto curMatch = _vTourMatches[i];
        std::string participant1, participant2;
        if (curMatch.participant_1_id.hasValue())
            participant1 = _pCoordinator.lock()->getMainAPI().userAPI()->getUsers(
                    {.id_ = curMatch.participant_1_id.getValue()})[0].nickname;
        if (curMatch.participant_2_id.hasValue())
            participant2 = _pCoordinator.lock()->getMainAPI().userAPI()->getUsers(
                    {.id_ = curMatch.participant_2_id.getValue()})[0].nickname;

        auto name = string_format("%10s vs %-10s", participant1.c_str(), participant2.c_str());
        auto key = string_format("%d", curMatch.id);

        std::string winner;
        if (curMatch.winner_id.hasValue())
            winner = _pCoordinator.lock()->getMainAPI().userAPI()->getUsers(
                    {.id_ = curMatch.winner_id.getValue()})[0].nickname;

        matchesStr[i] = convert(string_format("|%-24s|%-15s|%-6s|%-15s|",
                                              name.c_str(), curMatch.status.c_str(), key.c_str(), winner.c_str()));
    }
    _tourMatchesList =  newCDKScroll(_cdkScreen,
                                     getbegx(_participantsList->win) + _participantsList->boxWidth + 1,
                                     getbegy(_tourList->win) + _tourList->boxHeight, RIGHT,
                                     _participantsList->boxHeight - _tourList->boxHeight, 65,
                                     convert(label), (CDK_CSTRING2)matchesStr, _vTourMatches.size(), FALSE, A_REVERSE, TRUE, FALSE);
    return _tourMatchesList;
}

CDKBUTTON *polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::createStartTournamentButton() {

    if (_tournament.cur_participants_num == _tournament.max_participants_num ||
        _tournament.status != polytour::transport::Tournament::status_wait_for_participants())
        return newCDKButton(_cdkScreen, LEFT, getbegy(_participantsList->win) + _participantsList->boxHeight,
                            "</3>                                                   start                                                    <!3>",
                            nullptr, TRUE, FALSE);
    else
        return newCDKButton(_cdkScreen, LEFT, getbegy(_participantsList->win) + _participantsList->boxHeight,
                            "</2>                                                   start                                                    <!2>",
                            nullptr, TRUE, FALSE);
}

CDKBUTTON *polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::createToMainMenuButton() {
    return newCDKButton(_cdkScreen, LEFT, getbegy(_startButton->win) + _startButton->boxHeight,
                        "                                                  main menu                                                 ",
                        nullptr, TRUE, FALSE);
}

CDKBUTTON *polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::createToDeleteButton() {
    return newCDKButton(_cdkScreen, LEFT, getbegy(_toMainMenuButton->win) + _toMainMenuButton->boxHeight,
                        "</2>                                                   delete                                                   <!2>",
                        nullptr, TRUE, FALSE);
}

CDKLABEL *polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::createTournamentLabel() {
    std::vector<std::string> pre_name_text;
    pre_name_text.emplace_back(string_format("</C/4>Name: %20s<!C!4>", _tournament.name.c_str()));
    pre_name_text.emplace_back(string_format("</C/4>Status: %18s<!C!4>", _tournament.status.c_str()));
    const char* name_text[] = {convert(pre_name_text[0]), convert(pre_name_text[1])};
    auto* result = newCDKLabel(_cdkScreen, CENTER, getbegy(_logo->win) + _logo->boxHeight + 2,
                               (CDK_CSTRING2)name_text, 2, TRUE, FALSE);
    return result;
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::deactivateObj(EObjectType cdktype, void *object) {
    if (!object)
        return (FALSE);

    chtype esc = KEY_ESC;
    if (cdktype == vBUTTON)
        (void) activateCDKButton((CDKBUTTON*)object, &esc);
    else if (cdktype == vENTRY)
        (void) activateCDKEntry((CDKENTRY*) object, &esc);
    else if (cdktype == vSCROLL)
        (void) activateCDKScroll((CDKSCROLL *) object, &esc);
    else if (cdktype == vITEMLIST)
        (void) activateCDKItemlist((CDKITEMLIST*) object, &esc);
    else
        return (FALSE);
    return (TRUE);
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::increaseTour(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;

    auto* tourList = (CDKITEMLIST*)obj;
    int newItemId = tourList->currentItem + 1;
    if (newItemId == tourList->listSize) newItemId = 0;
    setCDKItemlistCurrentItem(tourList, newItemId);

    destroyCDKScroll(userdata->_tourMatchesList);
    userdata->_impl->createMatchesList();
    userdata->_impl->rebindMatches();

    newItemId = tourList->currentItem - 1;
    if (newItemId < 0) newItemId = tourList->listSize - 1;
    setCDKItemlistCurrentItem(tourList, newItemId);

    userdata->_impl->refresh();
    return 0;
}

void polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::refresh() {
    refreshCDKScreen(_cdkScreen);
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::decreaseTour(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    auto* tourList = (CDKITEMLIST*)obj;

    int newItemId = tourList->currentItem - 1;
    if (newItemId < 0) newItemId = tourList->listSize - 1;
    setCDKItemlistCurrentItem(tourList, newItemId);

    unbindCDKObject(objType, tourList, KEY_TAB);
    destroyCDKScroll(userdata->_tourMatchesList);
    userdata->_impl->createMatchesList();
    userdata->_impl->rebindMatches();

    newItemId = tourList->currentItem + 1;
    if (newItemId == tourList->listSize) newItemId = 0;
    setCDKItemlistCurrentItem(tourList, newItemId);

    userdata->_impl->refresh();
    return 0;
}

void polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::rebindMatches() {
    bindCDKObject (vITEMLIST, _tourList, KEY_TAB, toMatchSelection, &_userdata);
    bindCDKObject (vSCROLL, _tourMatchesList, KEY_TAB, toStartButton, &_userdata);
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::toMainMenu(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->_impl->_pCoordinator;
    userData->_impl->_finalCallback = [coordinator](){
        coordinator.lock()->toMainMenu();
    };
    userData->_impl->_isQuit = true;
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

char *polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::convert(const std::string &s) {
    char *pc = new char[s.size()+1];
    std::strcpy(pc, s.c_str());
    return pc;
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::toTourSelection(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_tourList](){
        activateCDKItemlist(obj, nullptr);
    };
    injectCDKScroll((CDKSCROLL*) obj, KEY_ESC);
    return FALSE;
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::toMatchSelection(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_tourMatchesList](){
        activateCDKScroll(obj, nullptr);
    };
    injectCDKItemlist((CDKITEMLIST *) obj, KEY_ESC);
    return FALSE;
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::toStartButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_startButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    if (objType == vBUTTON)
        return TRUE;
    return FALSE;
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::toMainMenuButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_toMainMenuButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::toDeleteButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_deleteButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::toMatchParticipants(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_participantsList](){
        activateCDKScroll(obj, nullptr);
    };
    injectCDKButton((CDKBUTTON *) obj, KEY_ESC);
    return TRUE;
}

void polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::popupParticipantLabel(const std::optional<transport::User>& user) {
    if (user.has_value()) {
        auto participant = user.value();
        int resultStringsCount = 3;
        const char *mesg[6];
        const char * formatStr = "%-50s";
        mesg[0] = convert(string_format(formatStr, "User profile"));
        std::string nickname = "Nickname: " + participant.nickname;
        mesg[1] = convert(string_format(formatStr, convert(nickname)));
        std::string name = "Name: " + participant.name;
        mesg[2] = convert(string_format(formatStr, convert(name)));
        std::string email = "Email: " + participant.email;
        mesg[3] = convert(string_format(formatStr, convert(email)));
        if (participant.surname.hasValue()) {
            resultStringsCount++;
            std::string surname = "Surname: " + participant.surname.getValue();
            mesg[resultStringsCount] = convert(string_format(
                    formatStr, convert(surname)));
        }
        if (participant.age.hasValue()) {
            resultStringsCount++;
            std::string age = "Age: " + std::to_string(participant.age.getValue());
            mesg[resultStringsCount] = convert(string_format(
                    formatStr, convert(age)));
        }
        resultStringsCount++;
        mesg[resultStringsCount] = "<C>Press any key to continue.";
        popupLabel(_cdkScreen, (CDK_CSTRING2) mesg, resultStringsCount);
    } else {
        const char *mesg[2];
        mesg[0] = "<C>Empty participant field";
        mesg[1] = "<C>Press any key to continue.";
        popupLabel(_cdkScreen, (CDK_CSTRING2) mesg, 2);
    }
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::startTournament(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->_impl->_pCoordinator;
    auto tournament = userData->_impl->_tournament;
    if (tournament.max_participants_num == tournament.cur_participants_num) {
        userData->_impl->_finalCallback = [coordinator, tournament = userData->_impl->_tournament]() {
            coordinator.lock()->getMainAPI().tournamentAPI()->start(tournament);
            auto updatedTournament = coordinator.lock()->getMainAPI().tournamentAPI()->getTournaments({
                .id_ = tournament.id
            })[0];
            coordinator.lock()->toTournament(updatedTournament);
        };
        userData->_impl->_isQuit = true;
        if (!deactivateObj(objType, obj)) return (FALSE);
    } else {
        const char *mesg[2];
        mesg[0] = "<C>Tournament isn't full";
        mesg[1] = "<C>Press any key to continue.";
        popupLabel(userData->_impl->_cdkScreen, (CDK_CSTRING2) mesg, 2);
    }
    return (TRUE);
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::chooseUser(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto selectedUserId = getCDKScrollCurrentItem(userData->_impl->_participantsList);
    userData->_impl->popupParticipantLabel(userData->_impl->_vParticipants[selectedUserId]);
    userData->_impl->_callback = []() {};
    return (TRUE);
}

int polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::deleteTournament(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->_impl->_pCoordinator;
    auto tournament = userData->_impl->_tournament;
    userData->_impl->_finalCallback = [coordinator, tournament = userData->_impl->_tournament]() {
        coordinator.lock()->getMainAPI().tournamentAPI()->remove(tournament);
        coordinator.lock()->toMainMenu();
    };
    userData->_impl->_isQuit = true;
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

template<typename... Args>
std::string polytour::ui::cdk::LeaderWaitTournamentWindow::Impl::string_format(const std::string &format, Args... args) {
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

polytour::ui::cdk::LeaderWaitTournamentWindow::LeaderWaitTournamentWindow(
        const std::shared_ptr<ICoordinator> &coordinator,
        const transport::Tournament& tournament):
        _pImpl(std::make_unique<Impl>(coordinator, tournament)){}

polytour::ui::cdk::LeaderWaitTournamentWindow::~LeaderWaitTournamentWindow() = default;

void polytour::ui::cdk::LeaderWaitTournamentWindow::destroy() {
    _pImpl->destroy();
}

void polytour::ui::cdk::LeaderWaitTournamentWindow::init() {
    _pImpl->init();
}
