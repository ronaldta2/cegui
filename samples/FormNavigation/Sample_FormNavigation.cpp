/***********************************************************************
    filename:   Sample_FormNavigation.cpp
    created:    30/5/2013
    author:     Timotei Dolean
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "Sample_FormNavigation.h"
#include "CEGUI/CEGUI.h"

#include <iostream>

using namespace CEGUI;
using namespace NavigationStrategiesPayloads;

/** This sample uses most of the code from the 'HelloWorld' sample. 
    Thus, most of the clarifying comments have been removed for brevity. **/

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool FormNavigationDemo::initialise(CEGUI::GUIContext* gui_context)
{
    d_usedFiles = CEGUI::String(__FILE__);

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    gui_context->getPointerIndicator().setDefaultImage("TaharezLook/MouseArrow");

    WindowManager& win_mgr = WindowManager::getSingleton();
    d_root = (DefaultWindow*)win_mgr.createWindow("DefaultWindow", "Root");

    Font& default_font = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    gui_context->setDefaultFont(&default_font);

    gui_context->setRootWindow(d_root);

    d_navigationStrategy = new LinearNavigationStrategy;
    d_windowNavigator = new WindowNavigator(getNavigationMappings(), d_navigationStrategy);
    gui_context->setWindowNavigator(d_windowNavigator);

    FrameWindow* wnd = (FrameWindow*)win_mgr.createWindow("TaharezLook/FrameWindow", 
        "Demo Window");
    d_root->addChild(wnd);

    wnd->setPosition(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.25f)));
    wnd->setSize(USize(cegui_reldim(0.5f), cegui_reldim( 0.5f)));
    wnd->setMaxSize(USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    wnd->setMinSize(USize(cegui_reldim(0.1f), cegui_reldim( 0.1f)));

    wnd->setText("Phony form");

    createForm(wnd);

    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void FormNavigationDemo::deinitialise()
{
}

void FormNavigationDemo::createForm(FrameWindow* wnd)
{
    wnd->addChild(createWidget("TaharezLook/Label", 0.0f, 0.0f, "Char name:"));
    wnd->addChild(createWidget("TaharezLook/Label", 0.0f, 0.1f, "Guild name:"));
    wnd->addChild(createWidget("TaharezLook/Label", 0.0f, 0.2f, "Initial gold:"));

    Window* editbox = createWidget("TaharezLook/Editbox", 0.2f, 0.0f);
    wnd->addChild(editbox);
    d_editboxes.push_back(editbox);
    d_navigationStrategy->d_windows.push_back(editbox);

    editbox = createWidget("TaharezLook/Editbox", 0.2f, 0.1f);
    wnd->addChild(editbox);
    d_editboxes.push_back(editbox);
    d_navigationStrategy->d_windows.push_back(editbox);

    editbox = createWidget("TaharezLook/Editbox", 0.2f, 0.2f);
    wnd->addChild(editbox);
    d_editboxes.push_back(editbox);
    d_navigationStrategy->d_windows.push_back(editbox);

    d_isGameMasterCheckbox = static_cast<ToggleButton*>(
        createWidget("TaharezLook/Checkbox", 0.01f, 0.3f, "Is Game Master"));
    d_isGameMasterCheckbox->setSize(USize(cegui_reldim(0.5f), cegui_reldim(0.1f)));
    wnd->addChild(d_isGameMasterCheckbox);
    d_navigationStrategy->d_windows.push_back(d_isGameMasterCheckbox);

    d_confirmButton = createWidget("TaharezLook/Button", 0.1f, 0.4f, "Confirm");
    d_confirmButton->subscribeEvent(PushButton::EventClicked, 
        Event::Subscriber(&FormNavigationDemo::disableConfirmButton, this));
    wnd->addChild(d_confirmButton);
    d_navigationStrategy->d_windows.push_back(d_confirmButton);

    Window* resetButton = createWidget("TaharezLook/Button", 0.3f, 0.4f, "Reset");
    resetButton->subscribeEvent(PushButton::EventClicked, 
        Event::Subscriber(&FormNavigationDemo::resetForm, this));
    wnd->addChild(resetButton);
    d_navigationStrategy->d_windows.push_back(resetButton);
}

CEGUI::Window* FormNavigationDemo::createWidget(const String& type, 
    float position_x, float position_y, const String& text)
{
    Window* widget = WindowManager::getSingleton().createWindow(type);

    widget->setText(text);
    widget->setPosition(UVector2(cegui_reldim(position_x), cegui_reldim(position_y)));

    return widget;
}

bool FormNavigationDemo::resetForm(const CEGUI::EventArgs& e)
{
    for(std::vector<Window*>::const_iterator itor = d_editboxes.begin(); 
        itor != d_editboxes.end(); ++itor)
    {
        (*itor)->setText("");
    }

    d_isGameMasterCheckbox->setSelected(false);
    d_confirmButton->setEnabled(true);

    return true;
}

bool FormNavigationDemo::disableConfirmButton(const CEGUI::EventArgs& e)
{
    d_confirmButton->setEnabled(false);

    return true;
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static FormNavigationDemo sample;
    return sample;
}

std::map<SemanticValue, String> FormNavigationDemo::getNavigationMappings()
{
    std::map<SemanticValue, String> mappings;

    mappings[SV_NavigateToNext] = NAVIGATE_NEXT;
    mappings[SV_NavigateToPrevious] = NAVIGATE_PREVIOUS;

    return mappings;
}
