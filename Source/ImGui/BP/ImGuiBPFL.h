// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <ImGui.h>
#include "Misc/Optional.h"
#include "ImGuiBPFL.generated.h"

/* structs */

// same as "ImGuiWindowFlags_" from "imgui.h"
UENUM(BlueprintType, meta = (Bitflags))
enum class EImGuiWindowFlags : uint8
{
	None UMETA(Hidden),
	NoTitleBar,					// Disable title-bar
	NoResize,					// Disable user resizing with the lower-right grip
	NoMove,						// Disable user moving the window
	NoScrollbar,				// Disable scrollbars (window can still scroll with mouse or programmatically)
	NoScrollWithMouse,			// Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
	NoCollapse,					// Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
	AlwaysAutoResize,			// Resize every window to its content every frame
	NoBackground,				// Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
	NoSavedSettings,			// Never load/save settings in .ini file
	NoMouseInputs,				// Disable catching mouse, hovering test with pass through.
	MenuBar,					// Has a menu-bar
	HorizontalScrollbar,		// Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
	NoFocusOnAppearing,			// Disable taking focus when transitioning from hidden to visible state
	NoBringToFrontOnFocus,		// Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
	AlwaysVerticalScrollbar,	// Always show vertical scrollbar (even if ContentSize.y < Size.y)
	AlwaysHorizontalScrollbar,	// Always show horizontal scrollbar (even if ContentSize.x < Size.x)
	AlwaysUseWindowPadding,		// Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
	NoNavInputs,				// No gamepad/keyboard navigation within the window
	NoNavFocus,					// No focusing toward this window with gamepad/keyboard navigation (e.g. skipped by CTRL+TAB)
	UnsavedDocument,			// Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.                    
};


// same as "ImGuiCond_" from "imgui.h"
UENUM(BlueprintType)
enum EImGuiConditions
{
	None UMETA(Hidden),		// No condition (always set the variable), same as _Always
	Always = 1 << 0,			// No condition (always set the variable)
	Once = 1 << 1,				// Set the variable once per runtime session (only the first call will succeed)
	FirstUseEver = 1 << 2,		// Set the variable if the object/window has no persistently saved data (no entry in .ini file)
	Appearing = 1 << 3			// Set the variable if the object/window is appearing after being hidden/inactive (or the first time)
};


UENUM(BlueprintType) //do poprawy, to chyba by³o u¿ywane tylko w blueprintach
enum ImGui_DragType
{
	Drag_Float		UMETA(DisplayName = "Float"),
	Drag_Int		UMETA(DisplayName = "Int"),
	Drag_Vector2D	UMETA(DisplayName = "Vector 2D"),
	Drag_Vector		UMETA(DisplayName = "Vector"),
	Drag_Vector4	UMETA(DisplayName = "Vector 4"),
	Drag_Rotator	UMETA(DisplayName = "Rotator"),
	Drag_Transform	UMETA(DisplayName = "Transform"),
	Drag_Color		UMETA(DisplayName = "Color")
};


// same as "ImGuiInputTextFlags_" from "imgui.h"
UENUM(BlueprintType, meta = (Bitflags))
enum class EImGuiInputTextFlags : uint8
{
	None UMETA(Hidden),
	CharsDecimal,					// Allow 0123456789.+-*/
	CharsHexadecimal,				// Allow 0123456789ABCDEFabcdef
	CharsUppercase,					// Turn a..z into A..Z
	CharsNoBlank,					// Filter out spaces, tabs
	AutoSelectAll,					// Select entire text when first taking mouse focus
	EnterReturnsTrue,				// Return 'true' when Enter is pressed (as opposed to every time the value was modified). Consider looking at the IsItemDeactivatedAfterEdit() function.
	CallbackCompletion,				// Callback on pressing TAB (for completion handling)
	CallbackHistory,				// Callback on pressing Up/Down arrows (for history handling)
	CallbackAlways,					// Callback on each iteration. User code may query cursor position, modify text buffer.
	CallbackCharFilter,				// Callback on character inputs to replace or discard them. Modify 'EventChar' to replace or discard, or return 1 in callback to discard.
	AllowTabInput,					// Pressing TAB input a '\t' character into the text field
	CtrlEnterForNewLine,			// In multi-line mode, unfocus with Enter, add new line with Ctrl+Enter (default is opposite: unfocus with Ctrl+Enter, add line with Enter).
	NoHorizontalScroll,				// Disable following the cursor horizontally
	AlwaysOverwrite,				// Overwrite mode
	ReadOnly,						// Read-only mode
	Password,						// Password mode, display all characters as '*'
	NoUndoRedo,						// Disable undo/redo. Note that input text owns the text data while active, if you want to provide your own undo/redo stack you need e.g. to call ClearActiveID().
	CharsScientific,				// Allow 0123456789.+-*/eE (Scientific notation input)
	CallbackResize,					// Callback on buffer capacity changes request (beyond 'buf_size' parameter value), allowing the string to grow. Notify when the string wants to be resized (for string types which hold a cache of their Size). You will be provided a new BufSize in the callback and NEED to honor it. (see misc/cpp/imgui_stdlib.h for an example of using this)
	CallbackEdit,					// Callback on any edit (note that InputText() already returns true on edit, the callback is useful mainly to manipulate the underlying buffer while focus is active)
	AlwaysInsertMode UMETA(Hidden)	// [renamed in 1.82] name was not matching behavior
};



// ! ALMOST the same as "ImGui_DirType" from "imgui.h"
// ImGui_DirType starts on -1, while EImGuiDirectionType on 0 
UENUM(BlueprintType)
enum EImGuiDirectionType 
{
	NoneDirection UMETA(DisplayName = "None"),
	Left,
	Right,
	Up,
	Down,
	COUNT UMETA(Hidden)
};



UCLASS()
class IMGUI_API UImGuiBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* - Functions for Blueprint Graphs -	*/

// Context creation and access

/* Main	*/

// Demo, Debug, Information		
// Styles

/* Windows */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !", meta = (AutoCreateRefTerm = "bOpen"))//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginMainWindow(FString Name, int WindowFlagsBitmask, bool bClosable, UPARAM(ref) bool& bOpen);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndMainWindow();

/* Child Windows */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bVisible") bool BeginChild(FString HashName, FVector2D Size, bool bBorder, int WindowFlagsBitmask);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndChild();

/* Windows Utilities */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static UPARAM(DisplayName = "bCollapsed") bool IsWindowCollapsed();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static UPARAM(DisplayName = "bFocused") bool IsWindowFocused();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static UPARAM(DisplayName = "bHovered") bool IsWindowHovered();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static UPARAM(DisplayName = "Position") FVector2D GetWindowPosition(bool bRelative = false);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static UPARAM(DisplayName = "Size") FVector2D GetWindowSize(bool bRelative = false);

/* Window manipulation */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetNextWindowScreenPosition(FVector2D ScreenPosition, EImGuiConditions Condition = EImGuiConditions::Always);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetNextWindowSize(FVector2D Size, EImGuiConditions Condition = EImGuiConditions::Always);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetNextWindowSizeConstraints(FVector2D SizeMin, FVector2D SizeMax);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetNextWindowContentSize(FVector2D Size);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetNextWindowCollapsed(bool bCollapsed, EImGuiConditions Condition = EImGuiConditions::Always);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetNextWindowFocused();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetNextWindowBackgroundAlpha(float BackgroundAlpha);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetWindowFontScale(float Scale = 1);

// Content region
// Windows Scrolling
// Parameters stacks (shared)
// Parameters stacks (current window)
// Style read access

/* Cursor / Layout */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void AddSeparator();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void StayInSameLine();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void AddSpacing();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void AddDummy(FVector2D Size);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void Indent(float ToRight = 0);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void BeginGroup();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndGroup();

// ID stack/scopes

/* Widgets / Text */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void AddText(FString Text);

/* Widgets / Main */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bClicked") bool Button(FString Label, FVector2D Size);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bClicked") bool SmallButton(FString Label);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bClicked") bool InvisibleButton(FString HashName, FVector2D Size);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bClicked") bool ArrowButton(FString HashName, EImGuiDirectionType Direction);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bChanged") bool Checkbox(FString Label, UPARAM(ref) bool& CheckedBool);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bClicked") bool RadioButton(FString Label, bool bActive);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bClicked") bool RadioButtonList(TSet<FString> Labels, UPARAM(ref) int& RadioedIntiger);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void AddProgressBar(float Fraction, FVector2D Size, FString Overlay);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void AddBullet();

/* Widgets / Combo Box */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginCombo(FString Label, FString Preview);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndCombo();

/* Widgets / Drag Sliders */  

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bChanged") bool AddDragFloatArray(FString Label, UPARAM(ref) TArray<float>& DraggedArrayReference, float DragSpeed = 1.0f, float MinValue = 0.0f, float MaxValue = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bChanged") bool AddDragIntArray(FString Label, UPARAM(ref) TArray<int>& DraggedArrayReference, float DragSpeed = 1.0f, int MinValue = 0, int MaxValue = 0);

/* Widgets / Regular Sliders */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bChanged") bool AddSliderFloatArray(FString Label, UPARAM(ref) TArray<float>& SlidedArrayReference, float MinValue = 0.0f, float MaxValue = 100.0f);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bChanged") bool AddSliderIntArray(FString Label, UPARAM(ref) TArray<int>& SlidedArrayReference, int MinValue = 0, int MaxValue = 100);


/* Widgets / Input with Keyboard */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bCallback") bool InputTextBox(FString Label, FString Hint, UPARAM(ref) FString& InputedString, int MaxCharactersCount, FVector2D BoxSize, int InputTextFlagsBitmask);

// Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little color square that can be left-clicked to open a picker, and right-clicked to open an option menu.)
/* Widgets / Trees */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool TreeNode(FString Label);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool CollapsingHeader(FString Label);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void SetNextItemOpen(bool bOpen = true, EImGuiConditions Condition = EImGuiConditions::Always);

/* Widgets / Selectables */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !", meta = (AutoCreateRefTerm = "bSelected"))//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bClicked") bool Selectable(FString Label, UPARAM(ref) bool& bSelected, FVector2D Size);

/* Widgets / List Boxes	*/

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginListBox(FString Label, FVector2D Size);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndListBox();

// Widgets: Data Plotting
// Widgets: Value() Helpers.

/* Widgets / Menus */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginMenuBar();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndMenuBar();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginMainMenuBar();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndMainMenuBar();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginMenu(FString Label, bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndMenu();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bActivated") bool MenuItem(FString Label, FString Shortcut, UPARAM(ref) bool& bSelected, bool bEnabled);

/* Tooltips	*/

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void BeginTooltip();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndTooltip();

/* Popups / begin/end functions */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginPopup(FString HashName, int WindowFlagsBitmask);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !", meta = (AutoCreateRefTerm = "bOpenModal"))//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginPopupModal(FString Name, bool bClosable, UPARAM(ref) bool& bOpenModal, int WindowFlagsBitmask);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndPopup();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void OpenPopup(FString HashName);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void CloseCurrentPopup();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !", BlueprintPure)
	static bool IsPopupOpen(FString HashName);

// Popups: open/close functions
// Popups: open+begin combined functions helpers
// Popups: query functions

/* Tables */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bOpen") bool BeginTable(FString HashName, int Column, FVector2D OuterSize, float InnerWidth);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndTable();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void TableNextRow(float MinRowHeight);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bVisible") bool TableNextColumn();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bVisible") bool TableSetColumnIndex(int Column);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void TableHeader(FString Label);

// Tables: Headers & Columns declaration
// Tables: Sorting
// Tables: Miscellaneous functions
// Legacy Columns API (prefer using Tables!)
/* Tab Bars, Tabs */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bVisible") bool BeginTabBar(FString HashName);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndTabBar();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !", meta = (AutoCreateRefTerm = "bOpen"))//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bSelected") bool BeginTabItem(FString Label, bool bClosable, UPARAM(ref) bool& bOpen);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndTabItem();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static UPARAM(DisplayName = "bClicked") bool TabItemButton(FString Label);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")
	static void SetNextTabItemClosed(FString Label);


// Logging/Capture

/* Drag and Drop */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static bool BeginDragDropSource();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static bool SetDragDropPayload(FString HashName);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndDragDropSource();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static bool BeginDragDropTarget();

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static bool AcceptDragDropPayload(FString HashName);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndDragDropTarget();

/* Disabling [BETA API] */

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void BeginDisabled(bool bDisabled = true);

	UFUNCTION(BlueprintCallable, Category = "DearImGui|! Raw C++ Functions !")//, BlueprintInternalUseOnly)
	static void EndDisabled();

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

private:

    static ImVec2 GetScreenSizeInPixels(FVector2D ScreenSize = FVector2D(0, 0));

	static ImVec2 GetRelativeScreenPosition(FVector2D ScreenSize = FVector2D(0, 0));
};







