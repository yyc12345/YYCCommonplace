#include "dialog.hpp"
#if defined(YYCC_OS_WINDOWS) && defined(YYCC_STL_MSSTL)

#include "../string/op.hpp"
#include "../encoding/windows.hpp"
#include "../num/safe_cast.hpp"
#include "../num/safe_op.hpp"
#include <stdexcept>

#define ENC ::yycc::encoding::windows
#define OP ::yycc::string::op
#define SAFECAST ::yycc::num::safe_cast
#define SAFEOP ::yycc::num::safe_op
#define WINCOM ::yycc::windows::com

namespace yycc::windows::dialog {

#pragma region WinFileFilters

    WinFileFilters::WinFileFilters() : m_WinFilters(), m_WinDataStruct() {}

    WinFileFilters::~WinFileFilters() {}

    YYCC_IMPL_COPY_CTOR(WinFileFilters, rhs) : m_WinFilters(rhs.m_WinFilters), m_WinDataStruct() {
        // Update data
        this->update();
    }

    YYCC_IMPL_COPY_OPER(WinFileFilters, rhs) {
        // Copy data and mark desync
        this->m_WinFilters = rhs.m_WinFilters;
        this->m_WinDataStruct.clear();
        // Update data
        this->update();
        // Return self
        return *this;
    }

    YYCC_IMPL_MOVE_CTOR(WinFileFilters, rhs) : m_WinFilters(std::move(rhs.m_WinFilters)), m_WinDataStruct() {
        // In theory, there is no update should perform,
        // however we do it because there is no guarantee that no memory allocation during this move.
        this->update();
    }

    YYCC_IMPL_MOVE_OPER(WinFileFilters, rhs) {
        // Move data
        this->m_WinFilters = std::move(rhs.m_WinFilters);
        this->m_WinDataStruct.clear();
        // Same reason for updating
        this->update();
        // Return self
        return *this;
    }

    UINT WinFileFilters::get_filter_count() const {
        // We have check this length when building this class,
        // so we can safely and directly cast it in there.
        return static_cast<UINT>(m_WinFilters.size());
    }

    const COMDLG_FILTERSPEC* WinFileFilters::get_filter_specs() const {
        return m_WinDataStruct.data();
    }

    void WinFileFilters::update() {
        // Make sure they have same size
        auto n = m_WinFilters.size();
        m_WinDataStruct.resize(n);

        // Assign data one by one
        for (auto i = 0; i < n; ++i) {
            // Fetch item
            const auto& filter = m_WinFilters[i];
            auto& data_struct = m_WinDataStruct[i];
            // Assign pointer
            data_struct.pszName = filter.first.c_str();
            data_struct.pszSpec = filter.second.c_str();
        }
    }

#pragma endregion

#pragma region FileFilters

    FileFilters::FileFilters() : m_Filters() {}

    FileFilters::~FileFilters() {}

    void FileFilters::add_filter(const std::u8string_view& filter_name, std::initializer_list<std::u8string_view> il) {
        // assign filter name
        FilterName name(filter_name);
        // check filter name
        if (name.empty()) {
            throw std::invalid_argument("filter name is empty");
        }

        // assign filter patterns
        FilterModes modes;
        for (const auto& item : il) {
            modes.emplace_back(item);
        }
        // check filter patterns
        if (modes.empty()) {
            throw std::invalid_argument("filter pattern list is empty");
        }

        // add into pairs and return
        m_Filters.emplace_back(std::make_pair(std::move(name), std::move(modes)));
    }

    size_t FileFilters::get_count() const {
        return m_Filters.size();
    }

    void FileFilters::clear() {
        m_Filters.clear();
    }

    DialogResult<WinFileFilters> FileFilters::to_windows() const {
        // prepare return value
        WinFileFilters rv;

        // check filter size
        // if it overflow the maximum value, return.
        size_t count = m_Filters.size();
        if (!SAFECAST::try_to<UINT>(count).has_value()) {
            return std::unexpected(DialogError::TooManyFilters);
        }

        // build new Windows oriented string vector
        for (const auto& item : m_Filters) {
            // convert name to wchar
            auto win_name = ENC::to_wchar(item.first).value();

            // join pattern string and convert to wchar
            const auto& modes = item.second;
            auto joined_modes = OP::join(modes.begin(), modes.end(), u8";");
            auto win_modes = ENC::to_wchar(joined_modes).value();

            // append this pair
            rv.m_WinFilters.emplace_back(std::make_pair(win_name, win_modes));
        }

        // update data struct
        rv.update();

        // okey, return value
        // please note that we do not check whether this list is empty in there.
        // this was done in generic_file_dialog() function.
        return rv;
    }

#pragma endregion

#pragma region WinFileDialog

    /**
     * @brief Assistant function for making copy of IShellItem*.
     * @param[in] obj The object for making copy.
     * @return Copied COM object.
     */
    static WINCOM::SmartIShellItem duplicate_shell_item(const WINCOM::SmartIShellItem& obj) {
        // COM object is actually like a std::shared_ptr.
        // So we simply copy its raw pointer and increase it reference counter if it is not nullptr.
        WINCOM::SmartIShellItem rv(obj.get());
        if (rv) rv->AddRef();
        // Okey, return copied object.
        return rv;
    }

    WinFileDialog::WinFileDialog() :
        m_WinOwner(NULL), m_WinFileTypes(), m_WinDefaultFileTypeIndex(0u), m_WinTitle(std::nullopt), m_WinInitFileName(std::nullopt),
        m_WinInitDirectory(nullptr) {}

    WinFileDialog::~WinFileDialog() {}

    YYCC_IMPL_COPY_CTOR(WinFileDialog, rhs) :
        m_WinOwner(rhs.m_WinOwner), m_WinFileTypes(rhs.m_WinFileTypes), m_WinDefaultFileTypeIndex(rhs.m_WinDefaultFileTypeIndex),
        m_WinTitle(rhs.m_WinTitle), m_WinInitFileName(rhs.m_WinInitFileName),
        m_WinInitDirectory(duplicate_shell_item(rhs.m_WinInitDirectory)) {}

    YYCC_IMPL_COPY_OPER(WinFileDialog, rhs) {
        this->m_WinOwner = rhs.m_WinOwner;
        this->m_WinFileTypes = rhs.m_WinFileTypes;
        this->m_WinDefaultFileTypeIndex = rhs.m_WinDefaultFileTypeIndex;
        this->m_WinTitle = rhs.m_WinTitle;
        this->m_WinInitFileName = rhs.m_WinInitFileName;
        this->m_WinInitDirectory = duplicate_shell_item(rhs.m_WinInitDirectory);

        return *this;
    }

    bool WinFileDialog::has_owner() const {
        return m_WinOwner != NULL;
    }

    HWND WinFileDialog::get_owner() const {
        if (!has_owner()) throw std::logic_error("fetch not set owner");
        return m_WinOwner;
    }

    bool WinFileDialog::has_title() const {
        return m_WinTitle.has_value();
    }

    const wchar_t* WinFileDialog::get_title() const {
        if (!has_title()) throw std::logic_error("fetch not set title");
        return m_WinTitle.value().c_str();
    }

    bool WinFileDialog::has_init_file_name() const {
        return m_WinInitFileName.has_value();
    }

    const wchar_t* WinFileDialog::get_init_file_name() const {
        if (!has_init_file_name()) throw std::logic_error("fetch not set init file name");
        return m_WinInitFileName.value().c_str();
    }

    bool WinFileDialog::has_init_directory() const {
        return m_WinInitDirectory.get() != nullptr;
    }

    IShellItem* WinFileDialog::get_init_directory() const {
        if (!has_init_file_name()) throw std::logic_error("fetch not set init directory");
        return m_WinInitDirectory.get();
    }

    const WinFileFilters& WinFileDialog::get_file_types() const {
        return m_WinFileTypes;
    }

    UINT WinFileDialog::get_default_file_type_index() const {
        // This index has been checked so we return it directly.
        return m_WinDefaultFileTypeIndex;
    }

#pragma endregion

#pragma region FileDialog

    FileDialog::FileDialog() :
        m_Owner(NULL), m_FileTypes(), m_DefaultFileTypeIndex(0u), m_Title(std::nullopt), m_InitFileName(std::nullopt),
        m_InitDirectory(std::nullopt) {}

    FileDialog::~FileDialog() {}

    void FileDialog::set_owner(HWND owner) {
        m_Owner = owner;
    }

    void FileDialog::set_title(const std::u8string_view& title) {
        m_Title = title;
    }

    void FileDialog::unset_title() {
        m_Title = std::nullopt;
    }

    void FileDialog::set_init_file_name(const std::u8string_view& init_filename) {
        m_InitFileName = init_filename;
    }

    void FileDialog::unset_init_file_name() {
        m_InitFileName = std::nullopt;
    }

    void FileDialog::set_init_directory(const std::u8string_view& init_dir) {
        m_InitDirectory = init_dir;
    }

    void FileDialog::unset_init_directory() {
        m_InitDirectory = std::nullopt;
    }

    FileFilters& FileDialog::configure_file_types() {
        return m_FileTypes;
    }

    void FileDialog::set_default_file_type_index(size_t idx) {
        m_DefaultFileTypeIndex = idx;
    }

    void FileDialog::clear() {
        m_Owner = NULL;
        m_Title = std::nullopt;
        m_InitFileName = std::nullopt;
        m_InitDirectory = std::nullopt;
        m_FileTypes.clear();
        m_DefaultFileTypeIndex = 0u;
    }

    DialogResult<WinFileDialog> FileDialog::to_windows() const {
        // prepare return value
        WinFileDialog rv;

        // set owner
        rv.m_WinOwner = m_Owner;

        // build title and init file name
        if (m_Title.has_value()) {
            rv.m_WinTitle = ENC::to_wchar(m_Title.value()).value();
        } else rv.m_WinTitle = std::nullopt;
        if (m_InitFileName.has_value()) {
            rv.m_WinInitFileName = ENC::to_wchar(m_InitFileName.value()).value();
        } else rv.m_WinInitFileName = std::nullopt;

        // fetch init directory
        if (m_InitDirectory.has_value()) {
            // convert to wchar path
            auto w_init_dir = ENC::to_wchar(m_InitDirectory.value()).value();

            // fetch IShellItem*
            // Ref: https://stackoverflow.com/questions/76306324/how-to-set-default-folder-for-ifileopendialog-interface
            IShellItem* init_directory = NULL;
            HRESULT hr = SHCreateItemFromParsingName(w_init_dir.c_str(), NULL, IID_PPV_ARGS(&init_directory));
            if (FAILED(hr)) return std::unexpected(DialogError::NoSuchDir);

            // assign IShellItem*
            rv.m_WinInitDirectory.reset(init_directory);
        } else rv.m_WinInitDirectory.reset();

        // build file filters
        auto win_file_types = m_FileTypes.to_windows();
        if (!win_file_types.has_value()) return std::unexpected(win_file_types.error());
        else rv.m_WinFileTypes = std::move(win_file_types.value());

        // check and assign default file type index
        // check whether it can be safely casted into UINT.
        auto win_def_index_base0 = SAFECAST::try_to<UINT>(m_DefaultFileTypeIndex);
        if (!win_def_index_base0.has_value()) {
            return std::unexpected(DialogError::IndexOverflow);
        }
        // check whether it can safely make addition with 1.
        auto win_def_index = SAFEOP::checked_add<UINT>(win_def_index_base0.value(), 1);
        if (!win_def_index.has_value()) {
            return std::unexpected(DialogError::IndexOverflow);
        }
        // okey, assign it.
        // please note we do not check the relation between this variable and file filters.
        // this was done in generic_file_dialog() function.
        rv.m_WinDefaultFileTypeIndex = win_def_index.value();

        // everything is okey
        return rv;
    }

#pragma endregion

#pragma region Exposed Functions

    enum class GenericFileDialogType { OpenFile, OpenFiles, SaveFile, OpenFolder };

    /**
     * @brief Extract display name from given IShellItem*.
     * @param[in] item The pointer to IShellItem for extracting.
     * @return Extract display name, or error occurs.
     * @remarks This is an assist function of generic_file_dialog().
    */
    static DialogResult<std::u8string> extract_display_name(IShellItem* item) {
        // fetch display name from IShellItem*
        LPWSTR display_name_ptr;
        HRESULT hr = item->GetDisplayName(SIGDN_FILESYSPATH, &display_name_ptr);
        if (FAILED(hr)) return std::unexpected(DialogError::BadCOMCall);
        WINCOM::SmartLPWSTR display_name(display_name_ptr);

        // convert result and return
        return ENC::to_utf8(display_name.get()).value();
    }

    /**
     * @brief Generic file dialog.
     * @details This function is the real underlying function of all dialog functions.
     * @param[in] params User specified parameter controlling the behavior of this file dialog, including title, file types and etc.
     * @return
     * The full path to user selected files or folders.
     * For multiple selection, the count of items >= 1. For others, the count of item must be 1.
     * Or nothing (click "Cancel"), or error occurs.
    */
    template<GenericFileDialogType EDialogType>
    static DialogResult<DialogOutcome<std::vector<std::u8string>>> generic_file_dialog(const FileDialog& params) {
        // Reference: https://learn.microsoft.com/en-us/windows/win32/shell/common-file-dialog
        // prepare result variable
        HRESULT hr;

        // create a const bad com call unexpected result
        // because it is widely used in this function.
        constexpr auto BAD_COM_CALL = std::unexpected(DialogError::BadCOMCall);

        // check whether COM environment has been initialized
        if (!WINCOM::is_initialized()) return BAD_COM_CALL;

        // create file dialog instance
        // fetch dialog CLSID first
        CLSID dialog_clsid;
        switch (EDialogType) {
            case GenericFileDialogType::OpenFile:
            case GenericFileDialogType::OpenFiles:
            case GenericFileDialogType::OpenFolder:
                dialog_clsid = CLSID_FileOpenDialog;
                break;
            case GenericFileDialogType::SaveFile:
                dialog_clsid = CLSID_FileSaveDialog;
                break;
            default:
                throw std::invalid_argument("unknown dialog type");
        }
        // create raw dialog pointer
        IFileDialog* raw_pfd = nullptr;
        hr = CoCreateInstance(dialog_clsid, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&raw_pfd));
        if (FAILED(hr)) return BAD_COM_CALL;
        // create memory-safe dialog pointer
        WINCOM::SmartIFileDialog pfd(raw_pfd);

        // set options for dialog
        // before setting, always get the options first in order.
        // not to override existing options.
        DWORD dwFlags;
        hr = pfd->GetOptions(&dwFlags);
        if (FAILED(hr)) return BAD_COM_CALL;
        // modify options
        switch (EDialogType) {
            // We want user only can pick file system files: FOS_FORCEFILESYSTEM.
            // Open dialog default: FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR
            // Save dialog default: FOS_OVERWRITEPROMPT | FOS_NOREADONLYRETURN | FOS_PATHMUSTEXIST | FOS_NOCHANGEDIR
            // Pick folder: FOS_PICKFOLDERS
            case GenericFileDialogType::OpenFile:
                dwFlags |= FOS_FORCEFILESYSTEM;
                dwFlags |= FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR;
                break;
            case GenericFileDialogType::OpenFiles:
                dwFlags |= FOS_FORCEFILESYSTEM;
                dwFlags |= FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR;
                dwFlags |= FOS_ALLOWMULTISELECT;
                break;
            case GenericFileDialogType::SaveFile:
                dwFlags |= FOS_FORCEFILESYSTEM;
                dwFlags |= FOS_OVERWRITEPROMPT | FOS_NOREADONLYRETURN | FOS_PATHMUSTEXIST | FOS_NOCHANGEDIR;
                break;
            case GenericFileDialogType::OpenFolder:
                dwFlags |= FOS_FORCEFILESYSTEM;
                dwFlags |= FOS_PATHMUSTEXIST | FOS_FILEMUSTEXIST | FOS_NOCHANGEDIR;
                dwFlags |= FOS_PICKFOLDERS;
                break;
            default:
                throw std::invalid_argument("unknown dialog type");
        }
        // set folder dialog options
        hr = pfd->SetOptions(dwFlags);
        if (FAILED(hr)) return BAD_COM_CALL;

        // build Windows used file dialog parameters
        auto pending_win_params = params.to_windows();
        if (!pending_win_params.has_value()) return std::unexpected(pending_win_params.error());
        WinFileDialog win_params(std::move(pending_win_params.value()));

        // setup title and init file name
        if (win_params.has_title()) {
            hr = pfd->SetTitle(win_params.get_title());
            if (FAILED(hr)) return BAD_COM_CALL;
        }
        if (win_params.has_init_file_name()) {
            hr = pfd->SetFileName(win_params.get_init_file_name());
            if (FAILED(hr)) return BAD_COM_CALL;
        }

        // setup init directory
        if (win_params.has_init_directory()) {
            hr = pfd->SetFolder(win_params.get_init_directory());
            if (FAILED(hr)) return BAD_COM_CALL;
        }

        // set file types and default file index when we picking file
        if constexpr (EDialogType != GenericFileDialogType::OpenFolder) {
            // fetch file filters
            const auto& file_filters = win_params.get_file_types();

            // first, check whether file filters is empty
            if (file_filters.get_filter_count() == 0) {
                return std::unexpected(DialogError::EmptyFilters);
            }
            // then validate index
            if (win_params.get_default_file_type_index() > file_filters.get_filter_count()) {
                return std::unexpected(DialogError::IndexOutOfRange);
            }

            // set file types list
            hr = pfd->SetFileTypes(file_filters.get_filter_count(), file_filters.get_filter_specs());
            if (FAILED(hr)) return BAD_COM_CALL;
            // set default file type index
            hr = pfd->SetFileTypeIndex(win_params.get_default_file_type_index());
            if (FAILED(hr)) return BAD_COM_CALL;
        }

        // show the dialog and return if user click "Cancel"
        hr = pfd->Show(win_params.has_owner() ? win_params.get_owner() : NULL);
        if (FAILED(hr)) return std::nullopt;

        // prepare return value
        std::vector<std::u8string> rv;
        // obtain result when user click "OK" button.
        switch (EDialogType) {
            case GenericFileDialogType::OpenFile:
            case GenericFileDialogType::OpenFolder:
            case GenericFileDialogType::SaveFile: {
                // obtain one file entry
                IShellItem* raw_item;
                hr = pfd->GetResult(&raw_item);
                if (FAILED(hr)) return BAD_COM_CALL;
                WINCOM::SmartIShellItem item(raw_item);

                // extract display name
                auto display_name = extract_display_name(item.get());
                if (!display_name.has_value()) return std::unexpected(display_name.error());

                // append result
                rv.emplace_back(std::move(display_name.value()));
            } break;
            case GenericFileDialogType::OpenFiles: {
                // try casting file dialog to file open dialog
                // Ref: https://learn.microsoft.com/en-us/windows/win32/learnwin32/asking-an-object-for-an-interface
                IFileOpenDialog* raw_pfod = nullptr;
                hr = pfd->QueryInterface(IID_PPV_ARGS(&raw_pfod));
                if (FAILED(hr)) return BAD_COM_CALL;
                WINCOM::SmartIFileOpenDialog pfod(raw_pfod);

                // obtain multiple file entires
                IShellItemArray* raw_items;
                hr = pfod->GetResults(&raw_items);
                if (FAILED(hr)) return BAD_COM_CALL;
                WINCOM::SmartIShellItemArray items(raw_items);

                // analyze file entries
                // get array count first
                DWORD items_count = 0u;
                hr = items->GetCount(&items_count);
                if (FAILED(hr)) return BAD_COM_CALL;
                // iterate array
                for (DWORD i = 0u; i < items_count; ++i) {
                    // fetch item by index
                    IShellItem* raw_item;
                    hr = items->GetItemAt(i, &raw_item);
                    if (FAILED(hr)) return BAD_COM_CALL;
                    WINCOM::SmartIShellItem item(raw_item);

                    // extract display name
                    auto display_name = extract_display_name(item.get());
                    if (!display_name.has_value()) return std::unexpected(display_name.error());

                    // append result
                    rv.emplace_back(std::move(display_name.value()));
                }
            } break;
            default:
                throw std::invalid_argument("unknown dialog type");
        }

        // everything is okey
        return rv;
    }

#pragma endregion

#pragma region Wrapper Functions

    /**
     * @brief Assistant function for extracting item from given value returned by generic file dialog.
     * @details This function will check whether inner vector only contain one item.
     * @param[in] rhs The return value to be extracted.
     * @return Extracted return value.
     */
    static DialogResult<DialogOutcome<std::u8string>> transform_generic_rv(DialogResult<DialogOutcome<std::vector<std::u8string>>>&& rhs) {
        if (rhs.has_value()) {
            const auto& inner = rhs.value();
            if (inner.has_value()) {
                const auto& vec = inner.value();
                if (vec.size() != 1u) throw std::logic_error("return value doesn't contain exactly one item");
                else return vec.front();
            } else {
                return std::nullopt;
            }
        } else {
            return std::unexpected(rhs.error());
        }
    }

    DialogResult<DialogOutcome<std::u8string>> open_file(const FileDialog& params){
        return transform_generic_rv(generic_file_dialog<GenericFileDialogType::OpenFile>(params));
    }

    DialogResult<DialogOutcome<std::vector<std::u8string>>> open_files(const FileDialog& params) {
        return generic_file_dialog<GenericFileDialogType::OpenFiles>(params);
    }

    DialogResult<DialogOutcome<std::u8string>> save_file(const FileDialog& params) {
        return transform_generic_rv(generic_file_dialog<GenericFileDialogType::SaveFile>(params));
    }

    DialogResult<DialogOutcome<std::u8string>> open_folder(const FileDialog& params) {
        return transform_generic_rv(generic_file_dialog<GenericFileDialogType::OpenFolder>(params));
    }

#pragma endregion

}

#endif
