    void start() override {
        m_sequence.frameMin = 0;
        m_sequence.frameMax = 100;
        m_sequence.items.push_back(CueSequence::Item{0, 0, 20, false});
    }

// update
if (ImGui::BeginTabItem("Sequencer##Tab")) {

    ImSequencer::Sequencer(&m_sequence, &m_currentFrame, &m_expanded, &m_selectedEntry, &m_firstFrame, 
            ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_CHANGE_FRAME);
    ImGui::EndTabItem();
}
ImGui::EndTabBar();

struct CueSequence : public ImSequencer::SequenceInterface {

    struct Item
    {
        int type;
        int frameStart, frameEnd;
        bool expanded;
    };

    int frameMin, frameMax;
    std::vector<Item> items;

    CueSequence() : frameMin(0), frameMax(0) {}

    virtual int GetFrameMin() const override { return frameMin; }
    virtual int GetFrameMax() const override { return frameMax; }
    virtual int GetItemCount() const override { return (int)items.size(); }
    virtual int GetItemTypeCount() const override { return 1; }
    virtual const char *GetItemTypeName(int typeIndex) const override {
        static char typeName[] = "TypeName";
        return typeName;
    }
    virtual const char *GetItemLabel(int index) const override {
        static char tmp[512];
        sprintf_s(tmp, "item_%02d", index);
        return tmp;
    }
    virtual void Add(int type) { items.push_back(Item{type, 0, 100, false}); };
    virtual void Del(int index) { items.erase(items.begin() + index); }
    virtual void Duplicate(int index) { items.push_back(items[index]); }
    
	virtual void Get(int index, int** start, int** end, int *type, unsigned int *color) {
        Item& item = items[index];
        if (color)
        	*color = 0x801080FF; // same color for everyone, return color based on type
		if (start)
			*start = &item.frameStart;
		if (end)
			*end   = &item.frameEnd;
		if (type)
			*type  = item.type;    
    }
};
