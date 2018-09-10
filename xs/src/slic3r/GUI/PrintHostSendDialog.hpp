#ifndef slic3r_PrintHostSendDialog_hpp_
#define slic3r_PrintHostSendDialog_hpp_

#include <string>
#include <boost/filesystem/path.hpp>

#include "slic3r/GUI/MsgDialog.hpp"

class wxTextCtrl;
class wxCheckBox;


namespace Slic3r {

class PrintHostSendDialog : public GUI::MsgDialog
{
private:
	wxTextCtrl *txt_filename;
	wxCheckBox *box_print;
	bool can_start_print;

public:
	PrintHostSendDialog(const boost::filesystem::path &path);
	std::string remote_path() const;          // returns path in utf-8
	bool print() const;                       // whether printing should be started

	virtual void EndModal(int ret);
};

}

#endif
