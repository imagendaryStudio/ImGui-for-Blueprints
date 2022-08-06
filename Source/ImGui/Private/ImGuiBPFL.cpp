// Fill out your copyright notice in the Description page of Project Settings.


#include "ImGuiBPFL.h"
#include <imgui.h>
#include "Misc/Optional.h"

#define MISSING_FLAG 0

//Public
// placeholders - test

void UImGuiBPFL::PrintSimpleWindow(FString Name, FString Text, FVector2D ScreenPosition)
{
	UImGuiBPFL::SetNextWindowScreenPosition(ScreenPosition, Once);
	ImGui::Begin(TCHAR_TO_ANSI(*Name), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_None);
	ImGui::Text(TCHAR_TO_ANSI(*Text));
	ImGui::End();
}

void UImGuiBPFL::PrintSimpleWatermark(FString Name, FString Text, FVector2D ScreenPosition, bool bPrintTextOnly, float BackgroundAlpha)
{	
	auto flags =
			ImGuiWindowFlags_AlwaysAutoResize +
			ImGuiWindowFlags_NoCollapse +
			ImGuiWindowFlags_NoInputs +
			ImGuiWindowFlags_NoFocusOnAppearing +
			ImGuiWindowFlags_NoNav +
			ImGuiWindowFlags_NoMove +
			ImGuiWindowFlags_None;

	if (bPrintTextOnly)
	{
		flags += ImGuiWindowFlags_NoTitleBar;
	}

	UImGuiBPFL::SetNextWindowScreenPosition(ScreenPosition, Always);
	ImGui::SetNextWindowBgAlpha(BackgroundAlpha);	
	ImGui::Begin(TCHAR_TO_ANSI(*Name), nullptr, flags);
	ImGui::Text(TCHAR_TO_ANSI(*Text));
	ImGui::End();
}


//Public

// Context creation and access

/* Main */

// Demo, Debug, Information	
	
// Styles

/* Windows */

bool UImGuiBPFL::BeginMainWindow(FString Name, TSet<TEnumAsByte<ImGui_WindowFlags>> Properties, bool bClosable, bool& bOpen)
{
	ImGuiWindowFlags PropertiesConverted = 0;
	bool* bOpenConverted = bClosable ? &bOpen : nullptr;

	for (ImGui_WindowFlags Flag : Properties)
	{
		PropertiesConverted += GetFixedWidnowFlag(Flag);
	}

	if (!bClosable || bOpen)
	{
		return ImGui::Begin(TCHAR_TO_ANSI(*Name), bOpenConverted, PropertiesConverted);
	}
	else
	{
		return false;
	}
}

void UImGuiBPFL::EndMainWindow()
{
	ImGui::End();
}

/* Child Windows */

bool UImGuiBPFL::BeginChild(FString HashName, FVector2D Size, bool bBorder, TSet<TEnumAsByte<ImGui_WindowFlags>> Properties)
{
	int HashId = GetTypeHash(HashName);
	ImVec2 SizeInPixels = GetScreenSizeInPixels(Size);
	ImGuiWindowFlags Flags = 0;

	for (ImGui_WindowFlags SimpleFlag : Properties)
	{
		Flags += GetFixedWidnowFlag(SimpleFlag);
	}

	return ImGui::BeginChild(HashId, SizeInPixels, bBorder, Flags);
}

void UImGuiBPFL::EndChild()
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
	return ImGui::IsWindowFocused(MISSING_FLAG);
}

bool UImGuiBPFL::IsWindowHovered()
{
	return ImGui::IsWindowHovered(MISSING_FLAG);
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
	{
		ImGui::SetNextWindowPos(WindowPosition, Condition, WindowPivot);
	}
}

void UImGuiBPFL::SetNextWindowSize(FVector2D Size, ImGui_WindowConditions Condition)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 SizeConverted = GetScreenSizeInPixels(Size);

	if (ViewportSize.X > 0 && ViewportSize.Y > 0)	 // don't call when viewport is just begin created
	{
		ImGui::SetNextWindowSize(SizeConverted, Condition);
	}
}

void UImGuiBPFL::SetNextWindowSizeConstraints(FVector2D SizeMin, FVector2D SizeMax)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 SizeMinConverted = GetScreenSizeInPixels(SizeMin);
	ImVec2 SizeMaxConverted = GetScreenSizeInPixels(SizeMax);

	if (ViewportSize.X > 0 && ViewportSize.Y > 0)	 // don't call when viewport is just begin created
	{
		ImGui::SetNextWindowSizeConstraints(SizeMinConverted, SizeMaxConverted);
	}
}

void UImGuiBPFL::SetNextWindowContentSize(FVector2D Size)
{
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	ImVec2 SizeConverted = GetScreenSizeInPixels(Size);;

	if (ViewportSize.X > 0 && ViewportSize.Y > 0)	 // don't call when viewport is just begin created
	{
		ImGui::SetNextWindowContentSize(SizeConverted);
	}
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

void UImGuiBPFL::AddDummy(FVector2D Size)
{
	ImVec2 SizeInPixels = GetScreenSizeInPixels(Size);

	ImGui::Dummy(SizeInPixels);
}

void UImGuiBPFL::Indent(float ToRight)
{
	ImGui::Indent(ToRight);
}

void UImGuiBPFL::BeginGroup()
{
	ImGui::BeginGroup();
}

void UImGuiBPFL::EndGroup()
{
	ImGui::EndGroup();
}

// ID stack/scopes


/* Widgets / Text */

void UImGuiBPFL::AddText(FString Text)
{
	ImGui::Text(TCHAR_TO_ANSI(*Text));
}

/* Widgets / Main */

bool UImGuiBPFL::Button(FString Label, FVector2D Size)
{
	ImVec2 SizeInPixels = GetScreenSizeInPixels(Size);

	return ImGui::Button(TCHAR_TO_ANSI(*Label), SizeInPixels);
}

bool UImGuiBPFL::SmallButton(FString Label)
{
	return ImGui::SmallButton(TCHAR_TO_ANSI(*Label));
}

bool UImGuiBPFL::InvisibleButton(FString HashName, FVector2D Size)
{
	ImVec2 SizeInPixels = GetScreenSizeInPixels(Size);

	return ImGui::InvisibleButton(TCHAR_TO_ANSI(*HashName), SizeInPixels);
}

bool UImGuiBPFL::ArrowButton(FString HashName, ImGui_DirType Direction)
{
	return ImGui::ArrowButton(TCHAR_TO_ANSI(*HashName), Direction - 1);
}

bool UImGuiBPFL::Checkbox(FString Label, bool& CheckedBool)
{
	return ImGui::Checkbox(TCHAR_TO_ANSI(*Label), &CheckedBool);
}

bool UImGuiBPFL::RadioButton(FString Label, bool bActive)
{
	return ImGui::RadioButton(TCHAR_TO_ANSI(*Label), bActive);
}

bool UImGuiBPFL::RadioButtonList(TSet<FString> Labels, int& RadioedIntiger)
{
	bool bRadioedIntigerChanged = false;
	int LabelIterator = 0;

	for (FString label : Labels)
	{
		if (ImGui::RadioButton(TCHAR_TO_ANSI(*label), &RadioedIntiger, LabelIterator))
		{
			bRadioedIntigerChanged = true; // this must be changed it branch, otherwise other buttons will set bRadioedIntigerChanged back to false
		}
		LabelIterator++;
	}

	return bRadioedIntigerChanged;
}

void UImGuiBPFL::AddProgressBar(float Fraction, FVector2D Size, FString Overlay)
{
	ImVec2 SizeInPixels = GetScreenSizeInPixels(Size);

	ImGui::ProgressBar(
		Fraction,
		SizeInPixels,
		Overlay.IsEmpty() ? nullptr : TCHAR_TO_ANSI(*Overlay)
	);
}

void UImGuiBPFL::AddBullet()
{
	ImGui::Bullet();
}

/* Widgets: Combo Box */

bool UImGuiBPFL::BeginCombo(FString Label, FString Preview)
{
	return ImGui::BeginCombo(TCHAR_TO_ANSI(*Label), TCHAR_TO_ANSI(*Preview));
}

void UImGuiBPFL::EndCombo()
{
	ImGui::EndCombo();
}

// Widgets: Drag Sliders

bool UImGuiBPFL::AddDragFloatArray(FString Label, TArray<float>& DraggedArrayReference, float DragSpeed, float MinValue, float MaxValue)
{
	int ItemsAmount = DraggedArrayReference.Num();
	float* PassByRefArray = new float[ItemsAmount];
	for (int i = 0; i < ItemsAmount; i++)
	{
		PassByRefArray[i] = DraggedArrayReference[i];
	}
	
	bool bChanged = ImGui::DragScalarN(TCHAR_TO_ANSI(*Label), ImGuiDataType_Float, PassByRefArray, ItemsAmount, DragSpeed, &MinValue, &MaxValue);
	for (int i = 0; i < ItemsAmount; i++)
	{
		DraggedArrayReference[i] = PassByRefArray[i];
	}

	delete[] PassByRefArray;
	return bChanged;
}

bool UImGuiBPFL::AddDragIntArray(FString Label, TArray<int>& DraggedArrayReference, float DragSpeed, int MinValue, int MaxValue)
{

	int ItemsAmount = DraggedArrayReference.Num();
	int* PassByRefArray = new int[ItemsAmount];
	for (int i = 0; i < ItemsAmount; i++)
	{
		PassByRefArray[i] = DraggedArrayReference[i];
	}

	bool bChanged = ImGui::DragScalarN(TCHAR_TO_ANSI(*Label), ImGuiDataType_S32, PassByRefArray, ItemsAmount, DragSpeed, &MinValue, &MaxValue);
	for (int i = 0; i < ItemsAmount; i++)
	{
		DraggedArrayReference[i] = PassByRefArray[i];
	}

	delete[] PassByRefArray;
	return bChanged;
}

/* Widgets / Regular Sliders */

bool UImGuiBPFL::AddSliderFloatArray(FString Label, TArray<float>& SlidedArrayReference, float MinValue, float MaxValue)
{
	int ItemsAmount = SlidedArrayReference.Num();
	float* PassByRefArray = new float[ItemsAmount];
	for (int i = 0; i < ItemsAmount; i++)
	{
		PassByRefArray[i] = SlidedArrayReference[i];
	}

	bool bChanged = ImGui::SliderScalarN(TCHAR_TO_ANSI(*Label), ImGuiDataType_Float, PassByRefArray, ItemsAmount, &MinValue, &MaxValue);
	for (int i = 0; i < ItemsAmount; i++)
	{
		SlidedArrayReference[i] = PassByRefArray[i];
	}

	delete[] PassByRefArray;
	return bChanged;
}

bool UImGuiBPFL::AddSliderIntArray(FString Label, TArray<int>& SlidedArrayReference, int MinValue, int MaxValue)
{
	int ItemsAmount = SlidedArrayReference.Num();
	int* PassByRefArray = new int[ItemsAmount];
	for (int i = 0; i < ItemsAmount; i++)
	{
		PassByRefArray[i] = SlidedArrayReference[i];
	}

	bool bChanged = ImGui::SliderScalarN(TCHAR_TO_ANSI(*Label), ImGuiDataType_S32, PassByRefArray, ItemsAmount, &MinValue, &MaxValue);
	for (int i = 0; i < ItemsAmount; i++)
	{
		SlidedArrayReference[i] = PassByRefArray[i];
	}

	delete[] PassByRefArray;
	return bChanged;
}

/* Widgets: Input with Keyboard	*/

bool UImGuiBPFL::InputTextBox(FString Label, FString Hint, FString& InputedString, int MaxCharactersCount, FVector2D BoxSize, TSet<TEnumAsByte<ImGui_InputTextType>> Properties)
{
	char* InputedStingConverted = TCHAR_TO_ANSI(*InputedString);
	int MaxCharactersCountConverted = MaxCharactersCount == 0 ? sizeof(InputedStingConverted) : MaxCharactersCount;
	ImVec2 BoxSizeConverted = GetScreenSizeInPixels(BoxSize);
	ImGuiInputTextFlags Flags = 0;
	for (ImGui_InputTextType SimpleFlag : Properties)
	{
		Flags += GetFixedInputTextFlag(SimpleFlag);
	}

	bool bCallback = ImGui::InputTextExSafe(TCHAR_TO_ANSI(*Label), TCHAR_TO_ANSI(*Hint), InputedStingConverted, MaxCharactersCountConverted, BoxSizeConverted, Flags);

	InputedString = FString(ANSI_TO_TCHAR(InputedStingConverted));
	return bCallback;
}

// Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
/* Widgets / Trees */

bool UImGuiBPFL::TreeNode(FString Label)
{
	return ImGui::TreeNode(TCHAR_TO_ANSI(*Label));
}

bool UImGuiBPFL::CollapsingHeader(FString Label)
{
	return ImGui::CollapsingHeader(TCHAR_TO_ANSI(*Label));
}

void UImGuiBPFL::SetNextItemOpen(bool bOpen, ImGui_WindowConditions Condition)
{
	ImGui::SetNextItemOpen(bOpen, Condition);
}

/* Widgets / Selectables */

bool UImGuiBPFL::Selectable(FString Label, bool& bSelected, FVector2D Size)
{
	ImVec2 SizeConverted = GetScreenSizeInPixels(Size);

	return ImGui::Selectable(TCHAR_TO_ANSI(*Label), &bSelected, MISSING_FLAG, SizeConverted);
}

/* Widgets / List Boxes	*/

bool UImGuiBPFL::BeginListBox(FString Label, FVector2D Size)
{
	ImVec2 SizeConverted = GetScreenSizeInPixels(Size);

	return ImGui::BeginListBox(TCHAR_TO_ANSI(*Label), SizeConverted);
}

void UImGuiBPFL::EndListBox()
{
	ImGui::EndListBox();
}

// Widgets: Data Plotting
// Widgets: Value() Helpers.
/* Widgets / Menus */

bool UImGuiBPFL::BeginMenuBar()
{
	return ImGui::BeginMenuBar();
}

void UImGuiBPFL::EndMenuBar()
{
	ImGui::EndMenuBar();
}

bool UImGuiBPFL::BeginMainMenuBar()
{
	return ImGui::BeginMainMenuBar();
}

void UImGuiBPFL::EndMainMenuBar()
{
	ImGui::EndMainMenuBar();
}

bool UImGuiBPFL::BeginMenu(FString Label, bool bEnabled)
{
	return ImGui::BeginMenu(TCHAR_TO_ANSI(*Label), bEnabled);
}

void UImGuiBPFL::EndMenu()
{
	ImGui::EndMenu();
}

bool UImGuiBPFL::MenuItem(FString Label, FString Shortcut, bool& bSelected, bool bEnabled)
{
	return ImGui::MenuItem(TCHAR_TO_ANSI(*Label), TCHAR_TO_ANSI(*Shortcut), &bSelected, bEnabled);
}

/* Tooltips */

void UImGuiBPFL::BeginTooltip()
{
	ImGui::BeginTooltip();
}

void UImGuiBPFL::EndTooltip()
{
	ImGui::EndTooltip();
}

/* Popups / begin/end functions */

bool UImGuiBPFL::BeginPopup(FString HashName, TSet<TEnumAsByte<ImGui_WindowFlags>> Properties)
{
	ImGuiWindowFlags PropertiesConverted = 0;

	for (ImGui_WindowFlags Flag : Properties)
	{
		PropertiesConverted += GetFixedWidnowFlag(Flag);
	}

	return ImGui::BeginPopup(TCHAR_TO_ANSI(*HashName), PropertiesConverted);
}

bool UImGuiBPFL::BeginPopupModal(FString Name, bool bClosable, bool& bOpenModal, TSet<TEnumAsByte<ImGui_WindowFlags>> Properties)
{
	bool* bOpenConverted = bClosable ? &bOpenModal : nullptr;
	ImGuiWindowFlags PropertiesConverted = 0;

	for (ImGui_WindowFlags Flag : Properties)
	{
		PropertiesConverted += GetFixedWidnowFlag(Flag);
	}

	return ImGui::BeginPopupModal(TCHAR_TO_ANSI(*Name), bOpenConverted, PropertiesConverted);
}

void UImGuiBPFL::EndPopup()
{
	ImGui::EndPopup();
}

void UImGuiBPFL::OpenPopup(FString HashName)
{
	if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))	 //TEMP, FIXME
	{
		ImGui::OpenPopup(TCHAR_TO_ANSI(*HashName), MISSING_FLAG);
	}
}

void UImGuiBPFL::CloseCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

bool UImGuiBPFL::IsPopupOpen(FString HashName)
{
	return ImGui::IsPopupOpen(TCHAR_TO_ANSI(*HashName), MISSING_FLAG);
}

// Popups: open/close functions
// Popups: open+begin combined functions helpers
// Popups: query functions
/* Tables */

bool UImGuiBPFL::BeginTable(FString HashName, int Column, FVector2D OuterSize, float InnerWidth)
{
	ImVec2 OuterSizeConverted = GetScreenSizeInPixels(OuterSize);

	return ImGui::BeginTable(TCHAR_TO_ANSI(*HashName), Column, MISSING_FLAG, OuterSizeConverted, InnerWidth);
}

void UImGuiBPFL::EndTable()
{
	ImGui::EndTable();
}

void UImGuiBPFL::TableNextRow(float MinRowHeight)
{
	ImGui::TableNextRow(MISSING_FLAG, MinRowHeight);
}

bool UImGuiBPFL::TableNextColumn()
{
	return ImGui::TableNextColumn();
}

bool UImGuiBPFL::TableSetColumnIndex(int Column)
{
	return ImGui::TableSetColumnIndex(Column);
}

void UImGuiBPFL::TableHeader(FString Label)
{
	ImGui::TableHeader(TCHAR_TO_ANSI(*Label));
}

// Tables: Headers & Columns declaration
// Tables: Sorting
// Tables: Miscellaneous functions
// Legacy Columns API (prefer using Tables!)
/* Tab Bars, Tabs */

bool UImGuiBPFL::BeginTabBar(FString HashName)
{
	return ImGui::BeginTabBar(TCHAR_TO_ANSI(*HashName));
}

void UImGuiBPFL::EndTabBar()
{
	ImGui::EndTabBar();
}

bool UImGuiBPFL::BeginTabItem(FString Label, bool bClosable, bool& bOpen)
{
	bool* bOpenConverted = bClosable ? &bOpen : nullptr;

	return ImGui::BeginTabItem(TCHAR_TO_ANSI(*Label), bOpenConverted);
}

void UImGuiBPFL::EndTabItem()
{
	ImGui::EndTabItem();
}

bool UImGuiBPFL::TabItemButton(FString Label)
{
	return ImGui::TabItemButton(TCHAR_TO_ANSI(*Label), 0);
}

void UImGuiBPFL::SetNextTabItemClosed(FString Label)
{
	ImGui::SetTabItemClosed(TCHAR_TO_ANSI(*Label));
}



// Logging/Capture

/* Drag and Drop */

bool UImGuiBPFL::BeginDragDropSource()
{
	return ImGui::BeginDragDropSource(MISSING_FLAG);
}

bool UImGuiBPFL::SetDragDropPayload(FString HashName)
{
	return ImGui::SetDragDropPayload(TCHAR_TO_ANSI(*HashName), nullptr, MISSING_FLAG);
}

void UImGuiBPFL::EndDragDropSource()
{
	ImGui::EndDragDropSource();
}

bool UImGuiBPFL::BeginDragDropTarget()
{
	return ImGui::BeginDragDropTarget();
}

bool UImGuiBPFL::AcceptDragDropPayload(FString HashName)
{
	const ImGuiPayload* Payload;

	Payload = ImGui::AcceptDragDropPayload(TCHAR_TO_ANSI(*HashName), MISSING_FLAG);
	return Payload ? true : false;
}

void UImGuiBPFL::EndDragDropTarget()
{
	ImGui::EndDragDropTarget();
}



/* Disabling [BETA API] */

void UImGuiBPFL::BeginDisabled(bool bDisabled)
{
	ImGui::BeginDisabled(bDisabled);
}

void UImGuiBPFL::EndDisabled()
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
	//ImGui::StyleColorsClassic();
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