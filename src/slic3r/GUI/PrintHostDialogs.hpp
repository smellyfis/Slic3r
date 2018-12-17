#ifndef slic3r_PrintHostSendDialog_hpp_
#define slic3r_PrintHostSendDialog_hpp_

#include <string>
#include <boost/filesystem/path.hpp>

#include <wx/string.h>
#include <wx/event.h>
#include <wx/dialog.h>

#include "GUI.hpp"
#include "GUI_Utils.hpp"
#include "MsgDialog.hpp"
#include "../Utils/PrintHost.hpp"

class wxTextCtrl;
class wxCheckBox;
class wxDataViewListCtrl;

namespace Slic3r {

struct PrintHostJob;

namespace GUI {


class PrintHostSendDialog : public GUI::MsgDialog
{
public:
    PrintHostSendDialog(const boost::filesystem::path &path);
    boost::filesystem::path filename() const;
    bool start_print() const;

private:
    wxTextCtrl *txt_filename;
    wxCheckBox *box_print;
    bool can_start_print;
};


class PrintHostQueueDialog : public wxDialog
{
public:
    class ProgressEvt : public wxEvent
    {
    public:
        enum State {
            ST_PROGRESS,
            ST_COMPLETE,
            ST_ERROR,
        };

        State state;
        size_t job_id;
        unsigned progress = 0;  // in percent
        wxString error;         // only non-empty if there was an error

        ProgressEvt(size_t job_id, State state) : job_id(job_id), state(state) {}
        ProgressEvt(size_t job_id, unsigned progress) : job_id(job_id), state(ST_PROGRESS), progress(progress) {}
        ProgressEvt(size_t job_id, wxString error) : job_id(job_id), state(ST_ERROR), error(std::move(error)) {}

        virtual wxEvent *Clone() const;
    };


    PrintHostQueueDialog(wxWindow *parent);

    void append_job(const PrintHostJob &job);

    // virtual int ShowModal();
private:
    wxDataViewListCtrl *job_list;
    int prev_width;    // XXX: remove
    EventGuard on_progress_evt;   // This prevents delivery of progress evts to a freed PrintHostQueueDialog

    void sanitize_col_widths();
    void on_progress(ProgressEvt&);
};


}}

#endif
