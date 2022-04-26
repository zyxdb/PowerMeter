/*============================================================================*/
/*             National Instruments / System Configuration API                */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 2010-2016.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:   nisyscfg_errors.h                                                 */
/* Purpose: Error codes for NI System Configuration API                       */
/*                                                                            */
/*============================================================================*/

#if !defined(_NI_SYSTEM_CONFIG_ERRORS_H_)
#define _NI_SYSTEM_CONFIG_ERRORS_H_

/**************************************************************************/
/* Status codes.                                                          */
/**************************************************************************/

#define NISysCfg_Succeeded(status)        ((status)>=0)   /* Check if the status indicates success */
#define NISysCfg_Failed(status)           ((status)<0)    /* Check if the status indicates failure */

typedef enum
{
   // Common success codes
   NISysCfg_OK                            = 0L,           /* The operation succeeded. */
   NISysCfg_EndOfEnum                     = 1L,           /* Reached end of the enumeration. Used by the NISysCfgNext* functions. */
   NISysCfg_SelfTestBasicOnly             = 263024L,      /* 0x00040370: The expert performed a basic self-test because it does not implement the specified mode. */
   NISysCfg_FoundCachedOfflineSystem      = 263168L,      /* 0x00040400: Initialization succeeded but the target is offline. Only cached system properties are available. */
   NISysCfg_RestartLocalhostInitiated     = 263169L,      /* 0x00040401: For the local system, the option to wait until the restart is complete is ignored. The function has successfully initiated a restart with the operating system. */

   // Common error codes
   NISysCfg_NotImplemented                = -2147467263L, /* 0x80004001: This operation is not implemented for this target or resource. */
   NISysCfg_NullPointer                   = -2147467261L, /* 0x80004003: A required pointer parameter was NULL. */
   NISysCfg_Fail                          = -2147467259L, /* 0x80004005: Miscellaneous operation failure. */
   NISysCfg_Unexpected                    = -2147418113L, /* 0x8000FFFF: A critical unexpected error occurred. Please report this to National Instruments. */
   NISysCfg_OutOfMemory                   = -2147024882L, /* 0x8007000E: Out of memory. */
   NISysCfg_InvalidArg                    = -2147024809L, /* 0x80070057: Some parameter is invalid. */
   NISysCfg_OperationTimedOut             = -2147220448L, /* 0x80040420: The operation timed out. */
   NISysCfg_FileNotFound                  = -2147220322L, /* 0x8004049E: The specified file was not found. */
   NISysCfg_InvalidMACFormat              = -2147220278L, /* 0x800404CA: Unsupported MAC address format. Supply the MAC address as a colon separated string of characters instead of Hex display. */

   // 'Hardware' and 'Resource' functions
   NISysCfg_PropMismatch                  = -2147220624L, /* 0x80040370: The property already exists with a different type or value. */
   NISysCfg_PropDoesNotExist              = -2147220623L, /* 0x80040371: The property does not exist for this resource. */
   NISysCfg_UriIllegalSyntax              = -2147220622L, /* 0x80040372: The name of the target or expert contains illegal characters or has an invalid format. Each label of the hostname must be between 1 and 63 characters long, and the entire hostname, including delimiting dots, must be 255 characters or less. */
   NISysCfg_UriTargetDoesNotExist         = -2147220621L, /* 0x80040373: Could not contact the NI System Configuration API at the specified target address. Ensure that the system is online. */
   NISysCfg_UriExpertDoesNotExist         = -2147220620L, /* 0x80040374: A specified expert is not installed. */
   NISysCfg_ItemDoesNotExist              = -2147220619L, /* 0x80040375: The specified resource name does not exist. */
   NISysCfg_InvalidMode                   = -2147220618L, /* 0x80040376: The specified mode is invalid. */
   NISysCfg_SysConfigAPINotInstalled      = -2147220616L, /* 0x80040378: The NI System Configuration API is not installed on the specified target. */
   NISysCfg_NameSyntaxIllegal             = -2147220614L, /* 0x8004037A: The suggested name contains illegal characters. */
   NISysCfg_NameCollision                 = -2147220613L, /* 0x8004037B: Another resource already has the suggested name. */
   NISysCfg_NoPropValidated               = -2147220612L, /* 0x8004037C: None of the changed properties could be validated. */
   NISysCfg_UriUnauthorized               = -2147220611L, /* 0x8004037D: The current user does not have permission for the requested operation. */
   NISysCfg_RenameResourceDependencies    = -2147220610L, /* 0x8004037E: The resource being renamed has dependencies, and the 'updateDependencies' flag was false. */
   NISysCfg_ValueInvalid                  = -2147220609L, /* 0x8004037F: A property contained a value that is not valid or is out of range. */
   NISysCfg_ValuesInconsistent            = -2147220608L, /* 0x80040380: Multiple properties contained values that are inconsistent with each other. */
   NISysCfg_Canceled                      = -2147220607L, /* 0x80040381: The operation was canceled. */
   NISysCfg_ResponseSyntax                = -2147220606L, /* 0x80040382: Could not parse the response from the NI System Configuration API at the specified target address. */
   NISysCfg_ResourceIsNotPresent          = -2147220605L, /* 0x80040383: The resource name is valid but the operation requires the resource to be present. */
   NISysCfg_ResourceIsSimulated           = -2147220604L, /* 0x80040384: The resource name is valid but the operation is not supported on simulated resources. */
   NISysCfg_NotInFirmwareUpdateState      = -2147220603L, /* 0x80040385: The resource requires being in the firmware update state to perform this operation. */
   NISysCfg_FirmwareImageDeviceMismatch   = -2147220602L, /* 0x80040386: The uploaded firmware image does not work with this resource. */
   NISysCfg_FirmwareImageCorrupt          = -2147220601L, /* 0x80040387: The uploaded firmware image is corrupt or incomplete. */
   NISysCfg_InvalidFirmwareVersion        = -2147220600L, /* 0x80040388: The specified firmware version does not exist. */
   NISysCfg_OlderFirmwareVersion          = -2147220599L, /* 0x80040389: The specified firmware version is older than what is currently installed. */
   NISysCfg_InvalidLoginCredentials       = -2147220598L, /* 0x8004038A: The username or password is incorrect. */
   NISysCfg_FirmwareUpdateAttemptFailed   = -2147220597L, /* 0x8004038B: The specified firmware was not successfully installed. See the output parameters for more information. */
   NISysCfg_EncryptionFailed              = -2147220596L, /* 0x8004038C: The data could not be encrypted. */
   NISysCfg_SomePropsNotValidated         = -2147220595L, /* 0x8004038D: The changes were not saved. Some of the modified properties were not validated because they do not apply to this item. */
   NISysCfg_InvalidCalibrationCredentials = -2147220594L, /* 0x8004038E: The calibration password is incorrect. */
   NISysCfg_CannotDeletePresentResource   = -2147220593L, /* 0x8004038F: Could not delete the specified resource because it is present. */
   NISysCfg_UriTargetTransmitError        = -2147220592L, /* 0x80040390: Failed transmitting data to or from the web server at the specified target address. */
   NISysCfg_DecryptionFailed              = -2147220591L, /* 0x80040391: The NI System Configuration API at the specified target address could not decrypt the data. */
   NISysCfg_FirmwareExpertVersionMismatch = -2147220590L, /* 0x80040392: The specified firmware requires a newer version of the expert than what is currently installed. */
   NISysCfg_AmbiguousImportAction         = -2147220589L, /* 0x80040393: There was uncertainty regarding what action to take during an import. */
   NISysCfg_RequiredItemFailedImport      = -2147220588L, /* 0x80040394: A required item could not be imported. */

   // 'Report' functions
   NISysCfg_PermissionDenied              = -2147220560L, /* 0x800403B0: Unable to write to file or folder. Permission denied. */
   NISysCfg_SystemNotFound                = -2147220559L, /* 0x800403B1: Unable to connect to the specified system. Ensure that the system is online. */
   NISysCfg_TransformFailed               = -2147220558L, /* 0x800403B2: Error running transform to generate report. */
   NISysCfg_NotInstalled                  = -2147220557L, /* 0x800403B3: Unable to find MAX on the system. Please reinstall. */
   NISysCfg_LaunchFailure                 = -2147220556L, /* 0x800403B4: Unexpected error launching nimax.exe. */
   NISysCfg_InternalTimeout               = -2147220555L, /* 0x800403B5: Launched nimax.exe but it did not complete in a reasonable time. */
   NISysCfg_MissingTransform              = -2147220554L, /* 0x800403B6: Unable to find an XSL transform to generate the report. */
   NISysCfg_IncorrectExtension            = -2147220553L, /* 0x800403B7: Incorrect report file extension provided. */
   NISysCfg_FileReadOnly                  = -2147220552L, /* 0x800403B8: Report file is read-only. Unable to generate report. */
   NISysCfg_ReportOverwrite               = -2147220551L, /* 0x800403B9: Report file exists and the NIMAX_FailIfOverwritingReport flag was set. */
   NISysCfg_DirectoryError                = -2147220550L, /* 0x800403BA: Error creating directory for report files. */

   // 'Export' and 'Import' functions
   NISysCfg_CannotOpenFile                = -2147220480L, /* 0x80040400: Error opening a file. */
   NISysCfg_InsufficientPermissions       = -2147220479L, /* 0x80040401: The object cannot be accessed because of insufficient permissions. */
   NISysCfg_NCECopierFailed               = -2147220478L, /* 0x80040402: Error with the object copier. */
   NISysCfg_FileOperationFailed           = -2147220477L, /* 0x80040403: Error performing a file operation. */
   NISysCfg_NameCollisionError            = -2147220476L, /* 0x80040404: Names from one expert have collided with another expert. */
   NISysCfg_UnexpectedError               = -2147220475L, /* 0x80040405: Unexpected error has occurred. */
   NISysCfg_NCENoStreamError              = -2147220474L, /* 0x80040406: The expert requested its stream for import but one does not exist because it did not export a stream. */
   NISysCfg_NCECompressionError           = -2147220473L, /* 0x80040407: Error compressing or decompressing file. */
   NISysCfg_NCEStreamReadError            = -2147220472L, /* 0x80040408: Error reading from a stream. */
   NISysCfg_NCEStreamWriteError           = -2147220471L, /* 0x80040409: Error writing to a stream. */
   NISysCfg_NCEStreamSeekError            = -2147220470L, /* 0x8004040A: Error seeking to a position in a stream. */
   NISysCfg_NCERepoNotReady               = -2147220469L, /* 0x8004040B: Repository not ready to be exported. */
   NISysCfg_NCERepoInvalid                = -2147220468L, /* 0x8004040C: The file or stream from which to import the repository is not a valid repository. */
   NISysCfg_NCERepoIncompat               = -2147220467L, /* 0x8004040D: The repository was exported with a newer version of MAX than what is on the importing machine. */
   NISysCfg_NCENoImportStorage            = -2147220466L, /* 0x8004040E: The import storage could not be opened. */
   NISysCfg_NCENoExportStorage            = -2147220465L, /* 0x8004040F: The export storage could not be created. */
   NISysCfg_NCENoObjCopier                = -2147220464L, /* 0x80040410: The object copier could not be created. */
   NISysCfg_CopyInProgress                = -2147220463L, /* 0x80040411: A PortCfg operation is already in progress. */
   NISysCfg_FileNotRecognized             = -2147220462L, /* 0x80040412: The custom file does not belong to a given expert. */
   NISysCfg_SystemNotSupported            = -2147220461L, /* 0x80040413: A specified system is not supported by this expert. */
   NISysCfg_SystemNotReachable            = -2147220460L, /* 0x80040414: A specified system is presumably supported, but network errors prevent connection. */
   NISysCfg_ProductSoftwareNotInstalled   = -2147220459L, /* 0x80040415: The product is not installed on the specified system. */
   NISysCfg_ProductSoftwareTooOld         = -2147220458L, /* 0x80040416: The product is installed on the remote system, but is too old. */
   NISysCfg_ProductSoftwareTooNew         = -2147220457L, /* 0x80040417: The product is installed on the remote system, but is too new. */
   NISysCfg_DataTooOld                    = -2147220456L, /* 0x80040418: The import data is too old. The product is not backward-compatible with this data. */
   NISysCfg_DataTooNew                    = -2147220455L, /* 0x80040419: The import data is too new. The product is not forward-compatible with this data. */
   NISysCfg_NoItemsToCopy                 = -2147220454L, /* 0x8004041A: The operation failed because no source items were specified. */
   NISysCfg_OrphanItems                   = -2147220453L, /* 0x8004041B: The operation failed because some items were orphans. */
   NISysCfg_DirtyItems                    = -2147220452L, /* 0x8004041C: The operation failed because some items were in-edit and not saved. */
   NISysCfg_FileOverwrite                 = -2147220451L, /* 0x8004041D: The operation failed because it would overwrite a file. */
   NISysCfg_ItemOverwrite                 = -2147220450L, /* 0x8004041E: The operation failed because it would overwrite items. */
   NISysCfg_MissingDependency             = -2147220449L, /* 0x8004041F: The operation failed because of missing dependency items. */
   NISysCfg_OperationCanceled             = -2147220447L, /* 0x80040421: The operation was canceled by the client. */
   NISysCfg_WarningConflicts              = -2147220446L, /* 0x80040422: The operation failed because of warning conflicts. */
   NISysCfg_ErrorConflicts                = -2147220445L, /* 0x80040423: The operation failed because of general conflicts. */
   NISysCfg_ItemsRequireUserInput         = -2147220444L, /* 0x80040424: The operation failed because of unresolved conflicts requiring user input. */
   NISysCfg_ProductExpertNotReady         = -2147220443L, /* 0x80040425: An expert is not ready to accept the specified source or destination, but may become ready in the future. */
   NISysCfg_OrphanFiles                   = -2147220442L, /* 0x80040426: The operation failed because some files were orphans. */
   NISysCfg_IsConst                       = -2147220441L, /* 0x80040427: Caller called a non-const method on an object that is logically const. */
   NISysCfg_UnsupportedProductMode        = -2147220440L, /* 0x80040428: An expert does not support the attempted copy mode (e.g. merge to file, etc.). */

   // 'System' functions
   NISysCfg_RestartLocalhostAmbiguous     = -2147220372L, /* 0x8004046C: To reboot your system, either specify 'localhost' on the front panel for Session in, or call Initialize Session first. */
   NISysCfg_ImageInvalidCorrupt           = -2147220371L, /* 0x8004046D: The image is corrupt or the file type is invalid. */
   NISysCfg_SafeOrInstallModeRequired     = -2147220370L, /* 0x8004046E: Can only perform this action in safe or install mode, and the 'auto restart' flag was false. */
   NISysCfg_EncryptPhraseMismatch         = -2147220369L, /* 0x8004046F: The encryption passphrase when applying an image was not the same as when the image was created. */
   NISysCfg_InvalidIP                     = -2147220368L, /* 0x80040470: IP Address is invalid. */
   NISysCfg_InvalidGateway                = -2147220367L, /* 0x80040471: Gateway is invalid. */
   NISysCfg_InvalidDNS                    = -2147220366L, /* 0x80040472: DNS is invalid. */
   NISysCfg_InvalidSubnet                 = -2147220365L, /* 0x80040473: Subnet is invalid. */
   NISysCfg_CmdNotSupported               = -2147220364L, /* 0x80040474: Command is not supported by given protocol. */
   NISysCfg_ConfigFailed                  = -2147220363L, /* 0x80040475: Remote system replied with the failure to config command. */
   NISysCfg_Locked                        = -2147220362L, /* 0x80040476: Remote system is locked. Requires a password to configure. */
   NISysCfg_BadPassword                   = -2147220361L, /* 0x80040477: The password supplied for the operation is invalid. */
   NISysCfg_NotConfigurable               = -2147220360L, /* 0x80040478: The remote device is not configurable for some reason other than password. */
   NISysCfg_UnlockFailed                  = -2147220359L, /* 0x80040479: Failed to unlock the system. */
   NISysCfg_LockFailed                    = -2147220358L, /* 0x8004047A: Failed to lock the system. */
   NISysCfg_InstallFailed                 = -2147220357L, /* 0x8004047B: General installation failure. */
   NISysCfg_InstallationCorrupt           = -2147220356L, /* 0x8004047C: Installation component files were not found in the repository, or were corrupt. */
   NISysCfg_EmptyFile                     = -2147220355L, /* 0x8004047D: The installation file is empty. */
   NISysCfg_UnconfiguredIP                = -2147220354L, /* 0x8004047E: The system must have a valid IP before certain operations such as installation. Cannot be 0.0.0.0. */
   NISysCfg_InstallationGenericFailure    = -2147220352L, /* 0x80040480: General install error. */
   NISysCfg_DownloadAlreadyStarted        = -2147220350L, /* 0x80040482: Installation to the specified target has already started. Multiple simultaneous installations are not allowed. */
   NISysCfg_Aborted                       = -2147220349L, /* 0x80040483: Remote action aborted. */
   NISysCfg_DiskFull                      = -2147220338L, /* 0x8004048E: Hard Drive on the remote system is either full or has encountered an I/O error. */
   NISysCfg_HDFormatFailed                = -2147220337L, /* 0x8004048F: Hard Drive Format failed. */
   NISysCfg_HDFormatNotSafeMode           = -2147220336L, /* 0x80040490: System must be in safe mode before attempting Hard Drive Format. */
   NISysCfg_HDFormatRebootFailed          = -2147220335L, /* 0x80040491: System failed to reboot after Hard Drive format. System is in unknown state. */
   NISysCfg_ConnectionRefused             = -2147220334L, /* 0x80040492: The server refused the network connection. */
   NISysCfg_GetRemoteFilesFailed          = -2147220331L, /* 0x80040495: Failed to get one or more files while creating system image. */
   NISysCfg_PutRemoteFilesFailed          = -2147220330L, /* 0x80040496: Failed to put one or more files while applying system image. */
   NISysCfg_InvalidImage                  = -2147220329L, /* 0x80040497: The specified path does not point to a valid image. */
   NISysCfg_ImageDeviceCodeMismatch       = -2147220328L, /* 0x80040498: The image is for a different device class and is incompatible with the target. */
   NISysCfg_SystemMismatch                = -2147220327L, /* 0x80040499: The image was not originally created from the specified target. */
   NISysCfg_HDFormatWrongFS               = -2147220326L, /* 0x8004049A: The requested file system is not supported on the specified target. */
   NISysCfg_CustomInstallNotSupported     = -2147220325L, /* 0x8004049B: The specified target does not support custom software installations. */
   NISysCfg_FTPFailed                     = -2147220324L, /* 0x8004049C: A file transfer error (FTP or WebDAV) occurred. */
   NISysCfg_Timeout                       = -2147220323L, /* 0x8004049D: Operation timed out. */
   NISysCfg_DirNotFound                   = -2147220321L, /* 0x8004049F: The specified directory was not found. */
   NISysCfg_PathNotFound                  = -2147220320L, /* 0x800404A0: The specified file or directory path was not found. */
   NISysCfg_NoSoftwareAvailable           = -2147220319L, /* 0x800404A1: No software is available for install. */
   NISysCfg_OverwriteError                = -2147220318L, /* 0x800404A2: The file or directory exists and the overwrite flag was false. */
   NISysCfg_HDFormatCannotKeepCfg         = -2147220317L, /* 0x800404A3: The target was not formatted because the option to keep configuration after a format is supported only for targets on the local subnet. To format the target, repeat the operation without requesting to keep the configuration. */
   NISysCfg_FileOrPathTooLong             = -2147220316L, /* 0x800404A4: Filename or pathname is longer than what the server supports. */
   NISysCfg_DDPInternalTimeout            = -2147220315L, /* 0x800404A5: Failed when communicating with the system. This issue is usually caused by a high latency in the network. Refer to KnowledgeBase article 42GH3O00 on ni.com for possible solutions. */
   NISysCfg_IOPermissionDenied            = -2147220314L, /* 0x800404A6: The operation failed because of insufficient permissions. */
   NISysCfg_PathAlreadyExists             = -2147220313L, /* 0x800404A7: The operation failed because the path already exists. */
   NISysCfg_ExecutionFailure              = -2147220312L, /* 0x800404A8: The execution of an external command, script, or application failed. */
   NISysCfg_DownloadError                 = -2147220311L, /* 0x800404A9: Failed to download the file from the 'RT Images' repository. */
   NISysCfg_NetSendFailed                 = -2147220309L, /* 0x800404AB: Failed to send command. */
   NISysCfg_ContactHostDisconnected       = -2147220308L, /* 0x800404AC: Could not contact remote target. Ensure that the system is online. */
   NISysCfg_NetSvcDown                    = -2147220307L, /* 0x800404AD: Could not access network. */
   NISysCfg_NotConfirmed                  = -2147220306L, /* 0x800404AE: Command was not confirmed. The result of the operation is uncertain. */
   NISysCfg_HostNotResolved               = -2147220305L, /* 0x800404AF: Hostname could not be resolved by DNS. */
   NISysCfg_RebootTimeout                 = -2147220304L, /* 0x800404B0: Timeout while waiting for reboot. System is offline. */
   NISysCfg_NoConfirmationFP1600          = -2147220303L, /* 0x800404B1: Sending new configuration operation returned a failure, but might not necessarily have failed. */
   NISysCfg_DuplicateStartup              = -2147220300L, /* 0x800404B4: Cannot install more than one startup component. */
   NISysCfg_RemoteInvalidArgument         = -2147220299L, /* 0x800404B5: Invalid argument passed. */
   NISysCfg_NotUninstallable              = -2147220298L, /* 0x800404B6: Cannot uninstall a component. There are dependencies. */
   NISysCfg_DuplicatesNotAllowed          = -2147220297L, /* 0x800404B7: Cannot install multiple packages of the same component. */
   NISysCfg_NotInstallable                = -2147220296L, /* 0x800404B8: Cannot install a component. There are dependencies. */
   NISysCfg_WrongDevice                   = -2147220295L, /* 0x800404B9: Component will not work on this target. */
   NISysCfg_WrongOS                       = -2147220294L, /* 0x800404BA: Component will not work on this operating system. */
   NISysCfg_OSVersionTooOld               = -2147220293L, /* 0x800404BB: A BIOS update is required before installing. */
   NISysCfg_IOError                       = -2147220292L, /* 0x800404BC: Cannot open file or folder. */
   NISysCfg_CorruptConfig                 = -2147220291L, /* 0x800404BD: Duplicate or missing components on target installation. */
   NISysCfg_BufferOverflow                = -2147220290L, /* 0x800404BE: Buffer overflow. Size is too small. */
   NISysCfg_UnsupportedCDFVersion         = -2147220289L, /* 0x800404BF: Unsupported version of CDF format. Needs a newer version of MAX or NI System Configuration. */
   NISysCfg_InvalidStack                  = -2147220288L, /* 0x800404C0: Invalid software set. */
   NISysCfg_IncompleteStack               = -2147220287L, /* 0x800404C1: Incompletely specified list of packages. Some hidden dependencies were added. */
   NISysCfg_StackItemMissing              = -2147220286L, /* 0x800404C2: One or more Software Set items could not be found in the repository. */
   NISysCfg_TopLevelHiddenComponentError  = -2147220285L, /* 0x800404C3: There is a top-level hidden component installed. */
   NISysCfg_InvalidAddon                  = -2147220284L, /* 0x800404C4: A component was passed in that is not an installable add-on. It may be an unknown ID, a defined item that is not an add-on, a missing add-on, or a non-installable add-on. */
   NISysCfg_NoRTImagesFolder              = -2147220283L, /* 0x800404C5: Could not find or access 'RT Images' repository location. */
   NISysCfg_NoRTImagesRegistry            = -2147220282L, /* 0x800404C6: Could not read the 'RT Images' registry key. */
   NISysCfg_NoRTS2CDF                     = -2147220281L, /* 0x800404C7: Could not find the rts2cdf conversion utility. */
   NISysCfg_UnsupportedOS                 = -2147220280L, /* 0x800404C8: The operating system is not supported. */
   NISysCfg_ExactVersionRequired          = -2147220279L, /* 0x800404C9: Unspecified version while trying to install exact version of a component. */
   NISysCfg_InvalidStartup                = -2147220277L  /* 0x800404CB: A component was passed in that is not a startup. */
} NISysCfgStatus; 

#endif   // _NI_SYSTEM_CONFIG_ERRORS_H_
