// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGuiBPFL.h"
#include <imgui.h>


//Public
// placeholders - test

void UImGuiBPFL::PrintSimpleWindow(FString Name, FString Text, FVector2D ScreenPosition)
{;
	char* NameConverted = TCHAR_TO_ANSI(*Name);
	char* TextConverted = TCHAR_TO_ANSI(*Text);

	UImGuiBPFL::SetNextWindowScreenPosition(ScreenPosition, Once);
	ImGui::Begin(NameConverted, nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_None);
	ImGui::Text(TextConverted);
	ImGui::End();
}

void UImGuiBPFL::PrintSimpleWatermark(FString Name, FString Text, FVector2D ScreenPosition, bool bPrintTextOnly, float BackgroundAlpha)
{	
	char* NameConverted = TCHAR_TO_ANSI(*Name);
	char* TextConverted = TCHAR_TO_ANSI(*Text);
	auto flags =
			ImGuiWindowFlags_AlwaysAutoResize +
			ImGuiWindowFlags_NoCollapse +
			ImGuiWindowFlags_NoInputs +
			ImGuiWindowFlags_NoFocusOnAppearing +
			ImGuiWindowFlags_NoNav +
			ImGuiWindowFlags_NoMove +
			ImGuiWindowFlags_None;
	if (bPrintTextOnly)
		flags += ImGuiWindowFlags_NoTitleBar;

	UImGuiBPFL::SetNextWindowScreenPosition(ScreenPosition, Always);
	ImGui::SetNextWindowBgAlpha(BackgroundAlpha);	
	ImGui::Begin(NameConverted, nullptr, flags);
	ImGui::Text(TextConverted);
	ImGui::End();
}


//Public

// Context creation and access

/* Main */

// Demo, Debug, Information	
	
// Styles

/* Windows */

void UImGuiBPFL::StartPrintingMainWindow(FString Name, TSet<TEnumAsByte<ImGui_WindowFlags>> Properties)
{
	char* NameConverted = TCHAR_TO_ANSI(*Name);

	ImGuiWindowFlags Flags = 0;
	for (ImGui_WindowFlags SimpleFlag : Properties)
		Flags += GetFixedWidnowFlag(SimpleFlag);

	ImGui::Begin(NameConverted, nullptr, Flags);
}

void UImGuiBPFL::StopPrintingMainWindow()
{
	ImGui::End();
}

/* Child Windows */

bool UImGuiBPFL::StartPrintingChild(FString HashName, FVector2D Size, bool bBorder, TSet<TEnumAsByte<ImGui_WindowFlags>> Properties)
{
	int HashId = GetTypeHash(HashName);
	ImVec2 SizeInPixels = GetScreenSizeInPixels(Size);
	ImGuiWindowFlags Flags = 0;
	for (ImGui_WindowFlags SimpleFlag : Properties)
		Flags += GetFixedWidnowFlag(SimpleFlag);

	return ImGui::BeginChild(HashId, SizeInPixels, bBorder, Flags);
}

void UImGuiBPFL::StopPrintingChild()
{
	ImGui::EndChild();
}

/* Windows Utilities */

bool UImGuiBPFL::IsWindowCollapsed()
{
	return ImGui::IsWindowCollapsed();
}

bool UImGuiBPFL::IsWindowFocused()
{
	return ImGui::IsWindowFocused((ImGuiFocusedFlags)0);
}

bool UImGuiBPFL::IsWindowHovered()
{
	return ImGui::IsWindowHovered((ImGuiFocusedFlags)0);
}

FVector2D UImGuiBPFL::GetWindowPosition(bool bRelative)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 WindowPositionRaw;
	FVector2D  WindowPositionFixed;

	WindowPositionRaw = ImGui::GetWindowPos();
	WindowPositionFixed.X = bRelative ? WindowPositionRaw.x / ViewportSize.X : WindowPositionRaw.x;
	WindowPositionFixed.Y = bRelative ? WindowPositionRaw.y / ViewportSize.Y : WindowPositionRaw.y;

	return  WindowPositionFixed;
}

FVector2D UImGuiBPFL::GetWindowSize(bool bRelative)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 WindowSizeRaw;
	FVector2D  WindowSizeFixed;

	WindowSizeRaw = ImGui::GetWindowSize();
	WindowSizeFixed.X = bRelative ? WindowSizeRaw.x / ViewportSize.X : WindowSizeRaw.x;
	WindowSizeFixed.Y = bRelative ? WindowSizeRaw.y / ViewportSize.Y : WindowSizeRaw.y;

	return  WindowSizeFixed;
}

/* Window manipulation */

void UImGuiBPFL::SetNextWindowScreenPosition(FVector2D ScreenPosition, ImGui_WindowConditions Condition)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 WindowPosition = GetScreenSizeInPixels(ScreenPosition);
	ImVec2 WindowPivot = GetRelativeScreenPosition(ScreenPosition);

	if (ViewportSize.X > 0 && ViewportSize.Y > 0)	// don't call when viewport is just begin created
		ImGui::SetNextWindowPos(WindowPosition, Condition, WindowPivot);
}

void UImGuiBPFL::SetNextWindowSize(FVector2D Size, ImGui_WindowConditions Condition)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 SizeConverted = GetScreenSizeInPixels(Size);

	if (ViewportSize.X > 0 && ViewportSize.Y > 0)	 // don't call when viewport is just begin created
		ImGui::SetNextWindowSize(SizeConverted, Condition);
}

void UImGuiBPFL::SetNextWindowSizeConstraints(FVector2D SizeMin, FVector2D SizeMax)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 SizeMinConverted = GetScreenSizeInPixels(SizeMin);
	ImVec2 SizeMaxConverted = GetScreenSizeInPixels(SizeMax);

	if (ViewportSize.X > 0 && ViewportSize.Y > 0)	 // don't call when viewport is just begin created
		ImGui::SetNextWindowSizeConstraints(SizeMinConverted, SizeMaxConverted);
}

void UImGuiBPFL::SetNextWindowContentSize(FVector2D Size)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 SizeConverted = GetScreenSizeInPixels(Size);;

	if (ViewportSize.X > 0 && ViewportSize.Y > 0)	 // don't call when viewport is just begin created
		ImGui::SetNextWindowContentSize(SizeConverted);
}

void UImGuiBPFL::SetNextWindowCollapsed(bool bCollapsed, ImGui_WindowConditions Condition)
{
	ImGui::SetNextWindowCollapsed(bCollapsed, Condition);
}

void UImGuiBPFL::SetNextWindowFocused()
{
	ImGui::SetNextWindowFocus();
}

void UImGuiBPFL::SetNextWindowBackgroundAlpha(float BackgroundAlpha)
{
	ImGui::SetNextWindowBgAlpha(BackgroundAlpha);
}

void UImGuiBPFL::SetWindowFontScale(float Scale)
{
	ImGui::SetWindowFontScale(Scale);
}

// Content region
// Windows Scrolling
// Parameters stacks (shared)
// Parameters stacks (current window)
// Style read access

/* Cursor / Layout */

void UImGuiBPFL::AddSeparator()
{
	ImGui::Separator();
}

void UImGuiBPFL::StayInSameLine()
{
	ImGui::SameLine();
}

void UImGuiBPFL::AddSpacing()
{
	ImGui::Spacing();
}

void UImGuiBPFL::Indent(float ToRight)
{
	ImGui::Indent(ToRight);
}


void UImGuiBPFL::StartPrintingGroup()
{
	ImGui::BeginGroup();
}

void UImGuiBPFL::StopPrintingGroup()
{
	ImGui::EndGroup();
}


// ID stack/scopes


/* Widgets / Text */

void UImGuiBPFL::AddText(FString Text)
{
	char* TextConverted = TCHAR_TO_ANSI(*Text);
	ImGui::Text(TextConverted);
}

/* Widgets / Main */

bool UImGuiBPFL::AddButton(FString Name, FVector2D Size)
{
	char* NameConverted = TCHAR_TO_ANSI(*Name);
	ImVec2 SizeInPixels = GetScreenSizeInPixels(Size);

	return ImGui::Button(NameConverted, SizeInPixels);
}

bool UImGuiBPFL::AddCheckbox(FString Label, bool& CheckedBool)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	return ImGui::Checkbox(LabelConverted, &CheckedBool);
}

bool UImGuiBPFL::AddRadioButton(FString Label, bool bActive)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	return ImGui::RadioButton(LabelConverted, bActive);
}

bool UImGuiBPFL::AddRadioButtonList(TSet<FString> Labels, int& RadioedIntiger)
{
	bool bRadioedIntigerChanged = false;
	int LabelIterator = 0;

	for (FString label : Labels)
	{
		char* labelConverted = TCHAR_TO_ANSI(*label);
		if (ImGui::RadioButton(labelConverted, &RadioedIntiger, LabelIterator))
			bRadioedIntigerChanged = true; // this must be changed it branch, otherwise other buttons will set bRadioedIntigerChanged back to false
		LabelIterator++;
	}

	return bRadioedIntigerChanged;
}

void UImGuiBPFL::AddProgressBar(float Fraction, FVector2D Size, FString Overlay)
{
	char* OverlayConverted = Overlay.IsEmpty() ? nullptr : TCHAR_TO_ANSI(*Overlay);
	ImVec2 SizeInPixels = GetScreenSizeInPixels(Size);

	ImGui::ProgressBar(Fraction, SizeInPixels, OverlayConverted);
}

void UImGuiBPFL::AddBullet()
{
	ImGui::Bullet();
}

/* Widgets: Combo Box */

bool UImGuiBPFL::StartPrintingCombo(FString Label, FString Preview)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	char* PreviewConverted = TCHAR_TO_ANSI(*Preview);

	return ImGui::BeginCombo(LabelConverted, PreviewConverted);
}

void UImGuiBPFL::StopPrintingCombo()
{
	ImGui::EndCombo();
}

// Widgets: Drag Sliders

bool UImGuiBPFL::AddDragFloatArray(FString Label, UPARAM(ref) TArray<float>& DraggedArrayReference, float DragSpeed, float MinValue, float MaxValue)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	int ItemsAmount = DraggedArrayReference.Num();
	float* PassByRefArray = new float[ItemsAmount];
	for (int i = 0; i < ItemsAmount; i++)
		PassByRefArray[i] = DraggedArrayReference[i];

	
	bool bChanged = ImGui::DragScalarN(LabelConverted, ImGuiDataType_Float, PassByRefArray, ItemsAmount, DragSpeed, &MinValue, &MaxValue);
	for (int i = 0; i < ItemsAmount; i++)
		DraggedArrayReference[i] = PassByRefArray[i];

	delete[] PassByRefArray;
	return bChanged;
}

bool UImGuiBPFL::AddDragIntArray(FString Label, UPARAM(ref) TArray<int>& DraggedArrayReference, float DragSpeed, int MinValue, int MaxValue)
{

	int ItemsAmount = DraggedArrayReference.Num();
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	int* PassByRefArray = new int[ItemsAmount];
	for (int i = 0; i < ItemsAmount; i++)
		PassByRefArray[i] = DraggedArrayReference[i];
	
	bool bChanged = ImGui::DragScalarN(LabelConverted, ImGuiDataType_S32, PassByRefArray, ItemsAmount, DragSpeed, &MinValue, &MaxValue);
	for (int i = 0; i < ItemsAmount; i++)
		DraggedArrayReference[i] = PassByRefArray[i];

	delete[] PassByRefArray;
	return bChanged;
}

/* Widgets / Regular Sliders */

bool UImGuiBPFL::AddSliderFloatArray(FString Label, UPARAM(ref) TArray<float>& SlidedArrayReference, float MinValue, float MaxValue)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	int ItemsAmount = SlidedArrayReference.Num();
	float* PassByRefArray = new float[ItemsAmount];
	for (int i = 0; i < ItemsAmount; i++)
		PassByRefArray[i] = SlidedArrayReference[i];


	bool bChanged = ImGui::SliderScalarN(LabelConverted, ImGuiDataType_Float, PassByRefArray, ItemsAmount, &MinValue, &MaxValue);
	for (int i = 0; i < ItemsAmount; i++)
		SlidedArrayReference[i] = PassByRefArray[i];

	delete[] PassByRefArray;
	return bChanged;
}

bool UImGuiBPFL::AddSliderIntArray(FString Label, UPARAM(ref) TArray<int>& SlidedArrayReference, int MinValue, int MaxValue)
{
	int ItemsAmount = SlidedArrayReference.Num();
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	int* PassByRefArray = new int[ItemsAmount];
	for (int i = 0; i < ItemsAmount; i++)
		PassByRefArray[i] = SlidedArrayReference[i];

	bool bChanged = ImGui::SliderScalarN(LabelConverted, ImGuiDataType_S32, PassByRefArray, ItemsAmount, &MinValue, &MaxValue);
	for (int i = 0; i < ItemsAmount; i++)
		SlidedArrayReference[i] = PassByRefArray[i];

	delete[] PassByRefArray;
	return bChanged;
}

/* Widgets: Input with Keyboard	*/

bool UImGuiBPFL::AddInputTextBox(FString Label, FString Hint, FString& InputedString, int MaxCharactersCount, FVector2D BoxSize, TSet<TEnumAsByte<ImGui_InputTextType>> Properties)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	char* HintConverted = TCHAR_TO_ANSI(*Hint);
	char* InputedStingConverted = TCHAR_TO_ANSI(*InputedString);
	int MaxCharactersCountConverted = MaxCharactersCount == 0 ? sizeof(InputedStingConverted) : MaxCharactersCount;
	ImVec2 BoxSizeConverted = GetScreenSizeInPixels(BoxSize);
	ImGuiInputTextFlags Flags = 0;
	for (ImGui_InputTextType SimpleFlag : Properties)
		Flags += GetFixedInputTextFlag(SimpleFlag);

	bool bCallback = ImGui::InputTextExSafe(LabelConverted, HintConverted, InputedStingConverted, MaxCharactersCountConverted, BoxSizeConverted, Flags);

	InputedString = FString(ANSI_TO_TCHAR(InputedStingConverted));
	return bCallback;
}

// Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
/* Widgets / Trees */

bool UImGuiBPFL::AddTreeNode(FString Label)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	return ImGui::TreeNode(LabelConverted);
}

bool UImGuiBPFL::AddCollapsingHeader(FString Label)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	return ImGui::CollapsingHeader(LabelConverted);
}

void UImGuiBPFL::SetNextItemOpen(bool bOpen, ImGui_WindowConditions Condition)
{
	ImGui::SetNextItemOpen(bOpen, Condition);
}

// Widgets: Selectables
/* Widgets / List Boxes	*/

bool UImGuiBPFL::StartPrintingListBox(FString Label, FVector2D Size)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	ImVec2 SizeConverted = GetScreenSizeInPixels(Size);

	return ImGui::BeginListBox(LabelConverted, SizeConverted);
}

void UImGuiBPFL::StopPrintingListBox()
{
	ImGui::EndListBox();
}

// Widgets: Data Plotting
// Widgets: Value() Helpers.
/* Widgets / Menus */

bool UImGuiBPFL::StartAddingToMenuBar()
{
	return ImGui::BeginMenuBar();
}

void UImGuiBPFL::StopAddingToMenuBar()
{
	ImGui::EndMenuBar();
}

bool UImGuiBPFL::StartPrintingMainMenuBar()
{
	return ImGui::BeginMainMenuBar();
}

void UImGuiBPFL::StopPrintingMainMenuBar()
{
	ImGui::EndMainMenuBar();
}

bool UImGuiBPFL::StartPrintingMenu(FString Label, bool bEnabled)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	return ImGui::BeginMenu(LabelConverted, bEnabled);
}

void UImGuiBPFL::StopPrintingMenu()
{
	ImGui::EndMenu();
}

bool UImGuiBPFL::AddMenuItem(FString Label, FString Shortcut, bool& bSelected, bool bEnabled)
{
	char* LabelConverted = TCHAR_TO_ANSI(*Label);
	char* ShortcutConverted = TCHAR_TO_ANSI(*Shortcut);
	return ImGui::MenuItem(LabelConverted, ShortcutConverted, &bSelected, bEnabled);
}

/* Tooltips */

void UImGuiBPFL::StartPrintingTooltip()
{
	ImGui::BeginTooltip();
}

void UImGuiBPFL::StopPrintingTooltip()
{
	ImGui::EndTooltip();
}

// Popups: begin/end functions
// Popups: open/close functions
// Popups: open+begin combined functions helpers
// Popups: query functions
// Tables
// Tables: Headers & Columns declaration
// Tables: Sorting
// Tables: Miscellaneous functions
// Legacy Columns API (prefer using Tables!)
// Tab Bars, Tabs
// Logging/Capture
// Drag and Drop
/* Disabling [BETA API] */

void UImGuiBPFL::StartPrintingDisabled(bool bDisabled)
{
	ImGui::BeginDisabled(bDisabled);
}

void UImGuiBPFL::StopPrintingDisabled()
{
	ImGui::EndDisabled();
}

// Clipping
// Focus, Activation
// Item/Widgets Utilities and Query Functions
// Viewports
// Miscellaneous Utilities
// Text Utilities
// Color Utilities
// Inputs Utilities: Keyboard
// Inputs Utilities: Mouse
// Clipboard Utilities
// Settings/.Ini Utilities
// Debug Utilities
// Memory Allocators


//TEST Func

void UImGuiBPFL::TestFunction()
{
	ImGui::Begin("Test Window", nullptr);
	static char teststring[] = "hello";
	ImGui::Text("hello");

	ImGui::End();
	//ImGui::InputText()
}

//Private

ImGuiWindowFlags UImGuiBPFL::GetFixedWidnowFlag(ImGui_WindowFlags Flag)
{
	switch (Flag)
	{
	case NoTitleBar:
		return ImGuiWindowFlags_NoTitleBar;
	case NoResize:
		return ImGuiWindowFlags_NoResize;
	case NoMove:
		return ImGuiWindowFlags_NoMove;
	case NoScrollbar:
		return ImGuiWindowFlags_NoScrollbar;
	case NoScrollWithMouse:
		return ImGuiWindowFlags_NoScrollWithMouse;
	case NoCollapse:
		return ImGuiWindowFlags_NoCollapse;
	case AlwaysAutoResize:
		return ImGuiWindowFlags_AlwaysAutoResize;
	case NoBackground:
		return ImGuiWindowFlags_NoBackground;
	case NoSavedSettings:
		return ImGuiWindowFlags_NoSavedSettings;
	case NoMouseInputs:
		return ImGuiWindowFlags_NoMouseInputs;
	case MenuBar:
		return ImGuiWindowFlags_MenuBar;
	case HorizontalScrollbar:
		return ImGuiWindowFlags_HorizontalScrollbar;
	case NoFocusOnAppearing:
		return ImGuiWindowFlags_NoFocusOnAppearing;
	case NoBringToFrontOnFocus:
		return ImGuiWindowFlags_NoBringToFrontOnFocus;
	case AlwaysVerticalScrollbar:
		return ImGuiWindowFlags_AlwaysVerticalScrollbar;
	case AlwaysHorizontalScrollbar:
		return ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	case AlwaysUseWindowPadding:
		return ImGuiWindowFlags_AlwaysUseWindowPadding;
	case NoNavInputs:
		return ImGuiWindowFlags_NoNavInputs;
	case NoNavFocus:
		return ImGuiWindowFlags_NoNavFocus;
	case UnsavedDocument:
		return ImGuiWindowFlags_UnsavedDocument;
	case NoNav:
		return ImGuiWindowFlags_NoNav;
	case NoDecoration:
		return ImGuiWindowFlags_NoDecoration;
	case NoInputs:
		return ImGuiWindowFlags_NoInputs;;
	case None:
		return ImGuiWindowFlags_None;
	default:
		UE_LOG(LogTemp, Warning, TEXT("'ImGuiWindowFlags UImGuiBPFL::GetFixedWidnowFlag(ImGui_WindowFlags Flag)' method just returned 'None' due to reching defailt case in switch on ImGui_WindowFlags. Probably new value was added to enum but not to switch."));
		return ImGuiWindowFlags_None;
		break;
	}
}

ImGuiInputTextFlags UImGuiBPFL::GetFixedInputTextFlag(ImGui_InputTextType Flag)
{
	switch (Flag)
	{
	case InputText_None:
		return ImGuiInputTextFlags_None;
	case InputText_CharsDecimal:
		return ImGuiInputTextFlags_CharsDecimal;
	case InputText_Uppercases:
		return ImGuiInputTextFlags_CharsUppercase;
	case InputText_NoBlanks:
		return ImGuiInputTextFlags_CharsNoBlank;
	case InputText_AutoSelectAll:
		return ImGuiInputTextFlags_AutoSelectAll;
	case InputText_EnterReturnsTrue:
		return ImGuiInputTextFlags_EnterReturnsTrue;
	case InputText_AllowTabInput:
		return ImGuiInputTextFlags_AllowTabInput;
	case InputText_CtrlEnterForNewLine:
		return ImGuiInputTextFlags_CtrlEnterForNewLine;
	case InputText_NoHorizontalScroll:
		return ImGuiInputTextFlags_NoHorizontalScroll;
	case InputText_ReadOnly:
		return ImGuiInputTextFlags_ReadOnly;
	case InputText_Password:
		return ImGuiInputTextFlags_Password;
	case InputText_NoUndoRedo:
		return ImGuiInputTextFlags_NoUndoRedo;
	case InputText_CharsScientific:
		return ImGuiInputTextFlags_CharsScientific;
	default:
		FString Message = "'ImGuiInputTextFlags UImGuiBPFL::GetFixedInputTextFlag(ImGui_InputTextType Flag)' method just returned 'None' due to reching default case in switch on ImGui_InputTextType. Probably new value was added to enum but not to switch.";
		GEngine->AddOnScreenDebugMessage((int32)(GetTypeHash(Message)), 0, FColor::Red, Message);
		return ImGuiWindowFlags_None;
		break;
	}
}

ImVec2 UImGuiBPFL::GetScreenSizeInPixels(FVector2D ScreenSize)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 Pixels;

	Pixels.x = ScreenSize.X < 0 || ScreenSize.X > ViewportSize.X ? 0 : ScreenSize.X <= 1 ? ScreenSize.X * ViewportSize.X : ScreenSize.X;
	Pixels.y = ScreenSize.Y < 0 || ScreenSize.Y > ViewportSize.Y ? 0 : ScreenSize.Y <= 1 ? ScreenSize.Y * ViewportSize.Y : ScreenSize.Y;

	return Pixels;
}

ImVec2 UImGuiBPFL::GetRelativeScreenPosition(FVector2D ScreenSize)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 RelativePosition;

	RelativePosition.x = ScreenSize.X < 0 || ScreenSize.X > ViewportSize.X ? 0 : ScreenSize.X <= 1 ? ScreenSize.X : ScreenSize.X / ViewportSize.X;
	RelativePosition.y = ScreenSize.Y < 0 || ScreenSize.Y > ViewportSize.Y ? 0 : ScreenSize.Y <= 1 ? ScreenSize.Y : ScreenSize.Y / ViewportSize.Y;

	return RelativePosition;
}