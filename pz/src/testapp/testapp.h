class TestApp : public wxApp {
	public:
		virtual bool OnInit();
		void ProcTool(wxCommandEvent &);
		DECLARE_EVENT_TABLE()
	protected:
	    wxFrame * frame;
        DrawWindow * dw;
};

DECLARE_APP(TestApp);

