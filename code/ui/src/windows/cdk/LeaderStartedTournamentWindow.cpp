//
// Created by denis on 10.08.2022.
//

#include "windows/cdk/LeaderStartedTournamentWindow.h"

#include <utility>
#include <cmath>
#include <cstring>
#include "cdk/cdk.h"

class polytour::ui::cdk::LeaderStartedTournamentWindow::Impl {
public:

    explicit Impl(const std::shared_ptr<ICoordinator>& coordinator, transport::Tournament tournament);
    ~Impl();

    void init();
    void destroy();

    static int deactivateObj(EObjectType cdktype GCC_UNUSED, void *object GCC_UNUSED);

    typedef struct UserData {
        Impl* _impl;
    } UserData;

    typedef struct MatchView {
        CDKLABEL* _matchViewLabel;
        CDKLABEL* _matchName;
        CDKLABEL* _matchStatus;
        CDKLABEL* _participant1Label;
        CDKBUTTON* _participant1ProfileButton;
        CDKLABEL* _participant1Tribe;
        CDKBUTTON* _participant1WinButton;
        CDKLABEL* _participant2Label;
        CDKBUTTON* _participant2ProfileButton;
        CDKLABEL* _participant2Tribe;
        CDKBUTTON* _participant2WinButton;
        CDKLABEL* _winnerLabel;
        CDKLABEL* _loserLabel;
        CDKBUTTON* _startMatchButton;
        transport::Match _match;
        bool _isDestroyed = true;
    } MatchView;

    static char *convert(const std::string & s);

    template<typename ... Args>
    std::string string_format( const std::string& format, Args ... args );

    CDKSCROLL* createParticipantsList();
    CDKITEMLIST* createTourList();
    CDKSCROLL * createMatchesList();
    CDKBUTTON * createDeleteButton();
    CDKLABEL * createTournamentLabel();
    CDKBUTTON * createToMainMenuButton();
    void createMatchView();
    void rebindMatches();
    void refresh();
    void removeMatchView();
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
    CDKBUTTON* _deleteButton;
    CDKITEMLIST* _tourList;
    CDKSCROLL* _tourMatchesList;
    CDKBUTTON* _toMainMenuButton;
    MatchView _matchView;

    std::vector<transport::User> _vParticipants;
    std::vector<transport::Match> _vTourMatches;
    std::optional<transport::User> _participant1;
    std::optional<transport::User> _participant2;

    static BINDFN_PROTO (toTourSelection);
    static BINDFN_PROTO (toMatchSelection);
    static BINDFN_PROTO (toDeleteButton);
    static BINDFN_PROTO (toMainMenuButton);
    static BINDFN_PROTO (toMatchParticipants);

    static BINDFN_PROTO (increaseTour);
    static BINDFN_PROTO (decreaseTour);
    static BINDFN_PROTO (chooseMatch);
    static BINDFN_PROTO (chooseUser);
    static BINDFN_PROTO (deleteTournament);
    static BINDFN_PROTO (toMainMenu);

    static BINDFN_PROTO (toParticipant1Button);
    static BINDFN_PROTO (toParticipant2Button);
    static BINDFN_PROTO (toParticipant1WinButton);
    static BINDFN_PROTO (toParticipant2WinButton);
    static BINDFN_PROTO (toStartMatchButton);
    static BINDFN_PROTO (activateParticipant1Button);
    static BINDFN_PROTO (activateParticipant2Button);
    static BINDFN_PROTO (activateParticipant1WinButton);
    static BINDFN_PROTO (activateParticipant2WinButton);
    static BINDFN_PROTO (activateStartMatchButton);
};

polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::Impl(const std::shared_ptr<ICoordinator> &coordinator,
                                                     transport::Tournament tournament):
        _pCoordinator(coordinator),
        _tournament(std::move(tournament)){
    _vParticipants = _pCoordinator.lock()->getMainAPI().tournamentAPI()->getTournamentParticipants(_tournament);
}

polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::~Impl() {
    destroy();
}

void polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::init() {
    _cdkScreen = initCDKScreen(nullptr);
    initCDKColor ();

    const char* label_text[] = {"               </B/K/5> Polytour <!B!K!5>                "};
    _logo = newCDKLabel(_cdkScreen, CENTER, TOP, (CDK_CSTRING2)label_text, 1, TRUE, TRUE);
    _tournamentName = createTournamentLabel();
    _participantsList = createParticipantsList();
    _tourList = createTourList();
    _tourMatchesList = createMatchesList();
    _deleteButton = createDeleteButton();
    _toMainMenuButton = createToMainMenuButton();

    _userdata._impl = this;
    _userdata._impl->_tourMatchesList = _tourMatchesList;

    bindCDKObject (vSCROLL, _participantsList, KEY_TAB, toTourSelection, &_userdata);
    bindCDKObject (vITEMLIST, _tourList, KEY_TAB, toMatchSelection, &_userdata);
    bindCDKObject (vSCROLL, _tourMatchesList, KEY_TAB, toDeleteButton, &_userdata);
    bindCDKObject (vBUTTON, _deleteButton, KEY_TAB, toMainMenuButton, &_userdata);
    bindCDKObject (vBUTTON, _toMainMenuButton, KEY_TAB, toMatchParticipants, &_userdata);

    bindCDKObject(vITEMLIST, _tourList, KEY_UP, increaseTour, &_userdata);
    bindCDKObject(vITEMLIST, _tourList, KEY_RIGHT, increaseTour, &_userdata);
    bindCDKObject(vITEMLIST, _tourList, KEY_DOWN, decreaseTour, &_userdata);
    bindCDKObject(vITEMLIST, _tourList, KEY_LEFT, decreaseTour, &_userdata);
    bindCDKObject(vBUTTON, _deleteButton, KEY_ENTER, deleteTournament, &_userdata);
    bindCDKObject(vBUTTON, _toMainMenuButton, KEY_ENTER, toMainMenu, &_userdata);
    bindCDKObject (vSCROLL, _tourMatchesList, KEY_ENTER, chooseMatch, &_userdata);
    bindCDKObject(vSCROLL, _participantsList, KEY_ENTER, chooseUser, &_userdata);

    refreshCDKScreen(_cdkScreen);
    activateCDKScroll(_participantsList, nullptr);

    do{
        _callback();
    } while(!_isQuit);

    destroy();
    _finalCallback();
}

void polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::destroy() {
    if (!_matchView._isDestroyed)
        removeMatchView();
    destroyCDKScroll(_participantsList);
    destroyCDKItemlist(_tourList);
    destroyCDKScroll(_tourMatchesList);
    destroyCDKButton(_deleteButton);
    destroyCDKButton(_toMainMenuButton);

    destroyCDKScreen(_cdkScreen);
    endCDK();
}

CDKSCROLL *polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::createParticipantsList() {
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

CDKITEMLIST *polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::createTourList() {
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
    sprintf(label, "%-67s", tour);
    auto* result = newCDKItemlist(_cdkScreen, getbegx(_participantsList->win) + _participantsList->boxWidth + 1,
                                  getbegy(_participantsList->win), "", label,
                                  (CDK_CSTRING2)toursStrRes, tours, 0, TRUE, FALSE);
    chtype ecs = KEY_ESC;
    activateCDKItemlist(result, &ecs);
    return result;
}

CDKSCROLL *polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::createMatchesList() {
    auto selectedTour = getCDKItemlistCurrentItem(_tourList) + 1;
    _vTourMatches = _pCoordinator.lock()->getMainAPI().matchAPI()->getMatches({
                                                                                      .tournament_id_ = _tournament.id,
                                                                                      .tour_ = selectedTour
                                                                              });

    std::string label = string_format("%-65s\n|%-24s|%-18s|%-6s|%-15s|", "Matches", "name", "status", "key", "winner");

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

        matchesStr[i] = convert(string_format("|%-24s|%-18s|%-6s|%-15s|",
                                              name.c_str(), curMatch.status.c_str(), key.c_str(), winner.c_str()));
    }
    _tourMatchesList =  newCDKScroll(_cdkScreen,
                                     getbegx(_participantsList->win) + _participantsList->boxWidth + 1,
                                     getbegy(_tourList->win) + _tourList->boxHeight, RIGHT,
                                     _participantsList->boxHeight - _tourList->boxHeight, 65,
                                     convert(label), (CDK_CSTRING2)matchesStr, _vTourMatches.size(), FALSE, A_REVERSE, TRUE, FALSE);
    return _tourMatchesList;
}

CDKBUTTON *polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::createDeleteButton() {
    return newCDKButton(_cdkScreen, LEFT, getbegy(_participantsList->win) + _participantsList->boxHeight,
                        "</2>                                                    delete                                                     <!2>",
                        nullptr, TRUE, FALSE);
}

CDKBUTTON *polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::createToMainMenuButton() {
    return newCDKButton(_cdkScreen, LEFT, getbegy(_deleteButton->win) + _deleteButton->boxHeight,
                        "                                                   main menu                                                   ",
                        nullptr, TRUE, FALSE);
}

CDKLABEL *polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::createTournamentLabel() {
    std::vector<std::string> pre_name_text;
    pre_name_text.emplace_back(string_format("</C/4>Name: %20s<!C!4>", _tournament.name.c_str()));
    pre_name_text.emplace_back(string_format("</C/4>Status: %18s<!C!4>", _tournament.status.c_str()));
    const char* name_text[] = {convert(pre_name_text[0]), convert(pre_name_text[1])};
    auto* result = newCDKLabel(_cdkScreen, CENTER, getbegy(_logo->win) + _logo->boxHeight + 2,
                               (CDK_CSTRING2)name_text, 2, TRUE, FALSE);
    return result;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::deactivateObj(EObjectType cdktype, void *object) {
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

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::increaseTour(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;

    auto* tourList = (CDKITEMLIST*)obj;
    int newItemId = tourList->currentItem + 1;
    if (newItemId == tourList->listSize) newItemId = 0;
    setCDKItemlistCurrentItem(tourList, newItemId);

    userdata->_impl->removeMatchView();
    destroyCDKScroll(userdata->_impl->_tourMatchesList);
    userdata->_impl->createMatchesList();
    userdata->_impl->rebindMatches();

    newItemId = tourList->currentItem - 1;
    if (newItemId < 0) newItemId = tourList->listSize - 1;
    setCDKItemlistCurrentItem(tourList, newItemId);

    userdata->_impl->refresh();
    return 0;
}

void polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::refresh() {
    refreshCDKScreen(_cdkScreen);
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::decreaseTour(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    auto* tourList = (CDKITEMLIST*)obj;

    int newItemId = tourList->currentItem - 1;
    if (newItemId < 0) newItemId = tourList->listSize - 1;
    setCDKItemlistCurrentItem(tourList, newItemId);

    userdata->_impl->removeMatchView();
    unbindCDKObject(objType, tourList, KEY_TAB);
    destroyCDKScroll(userdata->_impl->_tourMatchesList);
    userdata->_impl->createMatchesList();
    userdata->_impl->rebindMatches();

    newItemId = tourList->currentItem + 1;
    if (newItemId == tourList->listSize) newItemId = 0;
    setCDKItemlistCurrentItem(tourList, newItemId);

    userdata->_impl->refresh();
    return 0;
}

void polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::rebindMatches() {
    bindCDKObject (vITEMLIST, _tourList, KEY_TAB, toMatchSelection, &_userdata);
    bindCDKObject (vSCROLL, _tourMatchesList, KEY_TAB, toDeleteButton, &_userdata);
    bindCDKObject (vSCROLL, _tourMatchesList, KEY_ENTER, chooseMatch, &_userdata);
}

void polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::createMatchView() {
    auto startX = getbegx(_tourList->win) + _tourList->boxWidth;
    auto startY = getbegy(_tourList->win);

    // SAME WTF
    int endX = 0;
    int windowWidth = 0;
    {
        const char * tmp[] = {"text"};
        auto* label = newCDKLabel(_cdkScreen, RIGHT, TOP, (CDK_CSTRING2)tmp, 1, TRUE, FALSE);
        windowWidth = getbegx(label->win);
        endX = windowWidth;
        destroyCDKLabel(label);
    }
    windowWidth -= startX;

    auto windowHeight = getbegy(_deleteButton->win) - startY;

    auto selectedMatch = _vTourMatches[getCDKScrollCurrentItem(_tourMatchesList)];
    _matchView._match = selectedMatch;

    unbindCDKObject(vSCROLL, _userdata._impl->_tourMatchesList, KEY_TAB);

    transport::User participant1, participant2;
    if (selectedMatch.participant_1_id.hasValue()) {
        participant1 = _pCoordinator.lock()->getMainAPI().userAPI()->getUsers(
                {.id_ = selectedMatch.participant_1_id.getValue()})[0];
        _participant1 = participant1;
    }
    if (selectedMatch.participant_2_id.hasValue()) {
        participant2 = _pCoordinator.lock()->getMainAPI().userAPI()->getUsers(
                {.id_ = selectedMatch.participant_2_id.getValue()})[0];
        _participant2 = participant2;
    }

    auto name = string_format("%10s vs %-10s", participant1.nickname.c_str(), participant2.nickname.c_str());

    std::string winner;
    if (selectedMatch.winner_id.hasValue())
        winner = _pCoordinator.lock()->getMainAPI().userAPI()->getUsers(
                {.id_ = selectedMatch.winner_id.getValue()})[0].nickname;
    std::string loser;
    if (selectedMatch.loser_id.hasValue())
        loser = _pCoordinator.lock()->getMainAPI().userAPI()->getUsers(
                {.id_ = selectedMatch.loser_id.getValue()})[0].nickname;

    MatchView matchView;

    char* matchViewName[1];
    std::string matchViewNameFormat = "%-" + std::to_string(windowWidth - 2) + "s";
    matchViewName[0] = convert(string_format(convert(matchViewNameFormat), convert("Match info")));
    auto matchViewNameLabel = newCDKLabel(_cdkScreen, startX, startY, (CDK_CSTRING2) matchViewName, 1, TRUE, FALSE);

    char* matchName[1];
    std::string matchNameFormat = "Name: %-" + std::to_string(windowWidth - 8) + "s";
    matchName[0] = convert(string_format(convert(matchNameFormat), convert(name)));
    auto matchNameLabel = newCDKLabel(_cdkScreen, startX, getendy(matchViewNameLabel->win),
                                      (CDK_CSTRING2)matchName, 1, TRUE, FALSE);

    char* matchStatus[1];
    std::string matchStatusFormat = "Status: %-" + std::to_string(windowWidth - 10) + "s";
    matchStatus[0] = convert(string_format(convert(matchStatusFormat), convert(selectedMatch.status)));
    auto matchStatusLabel = newCDKLabel(_cdkScreen, startX, getendy(matchNameLabel->win),
                                        (CDK_CSTRING2)matchStatus, 1, TRUE, FALSE);

    // +++++++++++++++++++++++++++++++++++++++ Participant 1 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    const char* participant1LabelText[] = {"Participant 1: "};
    auto participant1Label = newCDKLabel(_cdkScreen, startX, getendy(matchStatusLabel->win),
                                         (CDK_CSTRING2)participant1LabelText, 1, TRUE, FALSE);
    auto participant1LinkButton = newCDKButton(
            _cdkScreen, getendx(participant1Label->win), getbegy(participant1Label->win),
            convert(string_format("%10s", participant1.nickname.c_str())), nullptr, TRUE, FALSE);
    _matchView._participant1ProfileButton = participant1LinkButton;
    bindCDKObject(vSCROLL, _userdata._impl->_tourMatchesList, KEY_TAB, toParticipant1Button, &_userdata);

    std::string participant1Tribe;
    if (selectedMatch.participant_1_tribe.hasValue())
        participant1Tribe = selectedMatch.participant_1_tribe.getValue();
    char* participant1TribeLabelText[1];
    std::string participantTribeLabelTextFormat;
    if (selectedMatch.status == transport::Match::status_started())
        participantTribeLabelTextFormat = "Tribe: %-" + std::to_string(endX - getendx(participant1LinkButton->win) - 16) + "s";
    else
        participantTribeLabelTextFormat = "Tribe: %-" + std::to_string(endX - getendx(participant1LinkButton->win) - 9) + "s";

    if (participant1Tribe.empty()) {
        participant1TribeLabelText[0] = convert(string_format(
                convert(participantTribeLabelTextFormat),
                convert("Not selected")));
    } else {
        participant1TribeLabelText[0] = convert(string_format(
                convert(participantTribeLabelTextFormat),
                convert(participant1Tribe)));
    }
    auto participant1TribeLabel = newCDKLabel(_cdkScreen, getendx(participant1LinkButton->win),
                                              getbegy(participant1LinkButton->win), (CDK_CSTRING2)participant1TribeLabelText,
                                              1, TRUE, FALSE);

    // +++++++++++++++++++++++++++++++++++++++++++++ Participant 2 ++++++++++++++++++++++++++++++++++++++++++++++++++

    const char* participant2LabelText[] = {"Participant 2: "};
    auto participant2Label = newCDKLabel(_cdkScreen, startX, getendy(participant1Label->win),
                                         (CDK_CSTRING2)participant2LabelText, 1, TRUE, FALSE);
    auto participant2LinkButton = newCDKButton(
            _cdkScreen, getendx(participant2Label->win), getbegy(participant2Label->win),
            convert(string_format("%10s", convert(participant2.nickname))), nullptr, TRUE, FALSE);
    _matchView._participant2ProfileButton = participant2LinkButton;
    bindCDKObject(vBUTTON, _matchView._participant1ProfileButton, KEY_TAB, toParticipant2Button, &_userdata);

    std::string participant2Tribe;
    if (selectedMatch.participant_2_tribe.hasValue())
        participant2Tribe = selectedMatch.participant_2_tribe.getValue();
    char* participant2TribeLabelText[1];
    if (participant2Tribe.empty())
        participant2TribeLabelText[0] = convert(string_format(
                convert(participantTribeLabelTextFormat),
                convert("Not selected")));
    else
        participant2TribeLabelText[0] = convert(string_format(
                convert(participantTribeLabelTextFormat),
                convert(participant2Tribe)));
    auto participant2TribeLabel = newCDKLabel(_cdkScreen, getendx(participant2LinkButton->win),
                                              getbegy(participant2LinkButton->win), (CDK_CSTRING2)participant2TribeLabelText,
                                              1, TRUE, FALSE);

    // Winner label
    char * winnerLabelText[1];
    std::string winnerLabelTextFormat = "Winner: %-" + std::to_string(windowWidth - 10) + "s";
    winnerLabelText[0] = convert(string_format(
            convert(winnerLabelTextFormat),
            convert(winner)));
    auto* winnerLabel = newCDKLabel(_cdkScreen, startX, getendy(participant2Label->win),
                                    (CDK_CSTRING2)winnerLabelText, 1, TRUE, FALSE);

    // Loser label
    char * loserLabelText[1];
    std::string loserLabelTextFormat = "Loser: %-" + std::to_string(windowWidth - 9) + "s";
    loserLabelText[0] = convert(string_format(
            convert(loserLabelTextFormat),
            convert(loser)));
    auto* loserLabel = newCDKLabel(_cdkScreen, startX, getendy(winnerLabel->win),
                                   (CDK_CSTRING2)loserLabelText, 1, TRUE, FALSE);

    _matchView._matchViewLabel = matchViewNameLabel;
    _matchView._matchName = matchNameLabel;
    _matchView._matchStatus = matchStatusLabel;
    _matchView._participant1Label = participant1Label;
    _matchView._participant1Tribe = participant1TribeLabel;
    _matchView._participant2Label = participant2Label;
    _matchView._participant2Tribe = participant2TribeLabel;
    _matchView._winnerLabel = winnerLabel;
    _matchView._loserLabel = loserLabel;
    _matchView._isDestroyed = false;

    if (selectedMatch.status == transport::Match::status_wait_tribes() &&
        selectedMatch.participant_1_tribe.hasValue() && selectedMatch.participant_2_tribe.hasValue()) {
        std::string format = "</3>%-" + std::to_string(windowWidth - 2) + "s<!3>";
        char* text = convert(string_format(convert(format), "start"));
        _matchView._startMatchButton = newCDKButton(_cdkScreen, getbegx(loserLabel->win), getendy(loserLabel->win),
                                                    text, nullptr, TRUE, FALSE);
        bindCDKObject(vBUTTON, _matchView._participant2ProfileButton, KEY_TAB, toStartMatchButton, &_userdata);
        bindCDKObject(vBUTTON, _matchView._startMatchButton, KEY_TAB, toDeleteButton, &_userdata);
        bindCDKObject(vBUTTON, _matchView._startMatchButton, KEY_ENTER, activateStartMatchButton, &_userdata);
    } else {
        bindCDKObject(vBUTTON, _matchView._participant2ProfileButton, KEY_TAB, toDeleteButton, &_userdata);
    }

    if (selectedMatch.status == transport::Match::status_started()) {
        _matchView._participant1WinButton = newCDKButton(
                _cdkScreen, getendx(participant1TribeLabel->win),
                getbegy(participant1TribeLabel->win), "</3> Win <!3>",
                nullptr, TRUE, FALSE);
        unbindCDKObject(vBUTTON, _matchView._participant1ProfileButton, KEY_TAB);
        bindCDKObject(vBUTTON, _matchView._participant1ProfileButton, KEY_TAB, toParticipant1WinButton, &_userdata);
        bindCDKObject(vBUTTON, _matchView._participant1WinButton, KEY_TAB, toParticipant2Button, &_userdata);
        bindCDKObject(vBUTTON, _matchView._participant1WinButton, KEY_ENTER, activateParticipant1WinButton, &_userdata);

        _matchView._participant2WinButton = newCDKButton(
                _cdkScreen, getendx(participant2TribeLabel->win),
                getbegy(participant2TribeLabel->win), "</3> Win <!3>",
                nullptr, TRUE, FALSE);
        unbindCDKObject(vBUTTON, _matchView._participant2ProfileButton, KEY_TAB);
        bindCDKObject(vBUTTON, _matchView._participant2ProfileButton, KEY_TAB, toParticipant2WinButton, &_userdata);
        bindCDKObject(vBUTTON, _matchView._participant2WinButton, KEY_TAB, toDeleteButton, &_userdata);
        bindCDKObject(vBUTTON, _matchView._participant2WinButton, KEY_ENTER, activateParticipant2WinButton, &_userdata);
    }

    bindCDKObject(vBUTTON, _matchView._participant1ProfileButton, KEY_ENTER, activateParticipant1Button, &_userdata);
    bindCDKObject(vBUTTON, _matchView._participant2ProfileButton, KEY_ENTER, activateParticipant2Button, &_userdata);
    refresh();
}

void polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::removeMatchView() {
    if (_matchView._isDestroyed)
        return;
    unbindCDKObject(vSCROLL, _userdata._impl->_tourMatchesList, KEY_TAB);
    bindCDKObject(vSCROLL, _userdata._impl->_tourMatchesList, KEY_TAB, toDeleteButton, &_userdata);
    _participant1 = std::nullopt;
    _participant2 = std::nullopt;
    destroyCDKLabel(_matchView._matchViewLabel);
    destroyCDKLabel(_matchView._matchName);
    destroyCDKLabel(_matchView._matchStatus);
    destroyCDKLabel(_matchView._participant1Label);
    destroyCDKButton(_matchView._participant1ProfileButton);
    destroyCDKLabel(_matchView._participant1Tribe);
    if (_matchView._participant1WinButton)
        destroyCDKButton(_matchView._participant1WinButton);
    destroyCDKLabel(_matchView._participant2Label);
    destroyCDKButton(_matchView._participant2ProfileButton);
    destroyCDKLabel(_matchView._participant2Tribe);
    if (_matchView._participant2WinButton)
        destroyCDKButton(_matchView._participant2WinButton);
    destroyCDKLabel(_matchView._winnerLabel);
    destroyCDKLabel(_matchView._loserLabel);
    if (_matchView._startMatchButton)
        destroyCDKButton(_matchView._startMatchButton);
    _matchView._isDestroyed = true;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::chooseMatch(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    if (!userdata->_impl->_matchView._isDestroyed)
        userdata->_impl->removeMatchView();
    userdata->_impl->createMatchView();
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toMainMenu(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto coordinator = userData->_impl->_pCoordinator;
    userData->_impl->_finalCallback = [coordinator](){
        coordinator.lock()->toMainMenu();
    };
    userData->_impl->_isQuit = true;
    if (!deactivateObj(objType, obj)) return (FALSE);
    return (TRUE);
}

char *polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::convert(const std::string &s) {
    char *pc = new char[s.size()+1];
    std::strcpy(pc, s.c_str());
    return pc;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toTourSelection(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_tourList](){
        activateCDKItemlist(obj, nullptr);
    };
    injectCDKScroll((CDKSCROLL*) obj, KEY_ESC);
    return FALSE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toMatchSelection(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_tourMatchesList](){
        activateCDKScroll(obj, nullptr);
    };
    injectCDKItemlist((CDKITEMLIST *) obj, KEY_ESC);
    return FALSE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toDeleteButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_deleteButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    if (objType == vBUTTON)
        return TRUE;
    return FALSE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toMainMenuButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_toMainMenuButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toMatchParticipants(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_participantsList](){
        activateCDKScroll(obj, nullptr);
    };
    injectCDKButton((CDKBUTTON *) obj, KEY_ESC);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toParticipant1Button(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_matchView._participant1ProfileButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toParticipant2Button(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_matchView._participant2ProfileButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toParticipant1WinButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_matchView._participant1WinButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toParticipant2WinButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_matchView._participant2WinButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::toStartMatchButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [obj = userdata->_impl->_matchView._startMatchButton](){
        activateCDKButton(obj, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::activateParticipant1Button(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->popupParticipantLabel(userdata->_impl->_participant1);
    return FALSE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::activateParticipant2Button(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->popupParticipantLabel(userdata->_impl->_participant2);
    return FALSE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::activateParticipant1WinButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [userdata](){
        auto match = userdata->_impl->_matchView._match;
        auto winner = userdata->_impl->_participant1.value();
        userdata->_impl->_pCoordinator.lock()->getMainAPI().matchAPI()->finish(match, winner);
        userdata->_impl->removeMatchView();
        destroyCDKScroll(userdata->_impl->_tourMatchesList);
        userdata->_impl->createMatchesList();
        userdata->_impl->rebindMatches();
        int matchListId;
        for(matchListId = 0; matchListId < userdata->_impl->_vTourMatches.size(); matchListId++) {
            if (match.id == userdata->_impl->_vTourMatches[matchListId].id)
                break;
        }
        setCDKScrollCurrentItem(userdata->_impl->_tourMatchesList, matchListId);
        userdata->_impl->createMatchView();
        activateCDKScroll(userdata->_impl->_tourMatchesList, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::activateParticipant2WinButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [userdata](){
        auto match = userdata->_impl->_matchView._match;
        auto winner = userdata->_impl->_participant2.value();
        userdata->_impl->_pCoordinator.lock()->getMainAPI().matchAPI()->finish(match, winner);
        userdata->_impl->removeMatchView();
        destroyCDKScroll(userdata->_impl->_tourMatchesList);
        userdata->_impl->createMatchesList();
        userdata->_impl->rebindMatches();
        int matchListId;
        for(matchListId = 0; matchListId < userdata->_impl->_vTourMatches.size(); matchListId++) {
            if (match.id == userdata->_impl->_vTourMatches[matchListId].id)
                break;
        }
        setCDKScrollCurrentItem(userdata->_impl->_tourMatchesList, matchListId);
        userdata->_impl->createMatchView();
        activateCDKScroll(userdata->_impl->_tourMatchesList, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::activateStartMatchButton(EObjectType objType, void * obj, void * data, chtype key) {
    auto userdata = (UserData*) data;
    userdata->_impl->_callback = [userdata](){
        auto match = userdata->_impl->_matchView._match;
        auto updatedMatch = match;
        updatedMatch.status = transport::Match::status_started();
        userdata->_impl->_pCoordinator.lock()->getMainAPI().matchAPI()->update(match, updatedMatch);
        userdata->_impl->removeMatchView();
        destroyCDKScroll(userdata->_impl->_tourMatchesList);
        userdata->_impl->createMatchesList();
        userdata->_impl->rebindMatches();
        int matchListId;
        for(matchListId = 0; matchListId < userdata->_impl->_vTourMatches.size(); matchListId++) {
            if (match.id == userdata->_impl->_vTourMatches[matchListId].id)
                break;
        }
        setCDKScrollCurrentItem(userdata->_impl->_tourMatchesList, matchListId);
        userdata->_impl->createMatchView();
        activateCDKScroll(userdata->_impl->_tourMatchesList, nullptr);
    };
    deactivateObj(objType, obj);
    return TRUE;
}

void polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::popupParticipantLabel(const std::optional<transport::User>& user) {
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

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::deleteTournament(EObjectType objType, void * obj, void * data, chtype key) {
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

int polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::chooseUser(EObjectType objType, void * obj, void * data, chtype key) {
    auto* userData = (UserData*)data;
    auto selectedUserId = getCDKScrollCurrentItem(userData->_impl->_participantsList);
    userData->_impl->popupParticipantLabel(userData->_impl->_vParticipants[selectedUserId]);
    userData->_impl->_callback = []() {};
    return (TRUE);
}

template<typename... Args>
std::string polytour::ui::cdk::LeaderStartedTournamentWindow::Impl::string_format(const std::string &format, Args... args) {
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

polytour::ui::cdk::LeaderStartedTournamentWindow::LeaderStartedTournamentWindow(
        const std::shared_ptr<ICoordinator> &coordinator,
        const transport::Tournament& tournament):
        _pImpl(std::make_unique<Impl>(coordinator, tournament)){}

polytour::ui::cdk::LeaderStartedTournamentWindow::~LeaderStartedTournamentWindow() = default;

void polytour::ui::cdk::LeaderStartedTournamentWindow::destroy() {
    _pImpl->destroy();
}

void polytour::ui::cdk::LeaderStartedTournamentWindow::init() {
    _pImpl->init();
}
