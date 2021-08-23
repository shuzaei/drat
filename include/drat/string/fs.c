/**
 * Functions that generate nicely formatted strings representing data found in
 * File-system-related objects, such as `apfs_superblock_t`.
 */

#include "fs.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <drat/string/common.h>
#include <drat/string/object.h>

/**
 * Get a human-readable string that lists the optional feature flags that are
 * set on a given APFS volume superblock.
 * 
 * apsb:    A pointer to the APFS volume superblock in question.
 * 
 * RETURN VALUE:
 *      A pointer to the first character of the string. The caller must free
 *      this pointer when it is no longer needed.
 */
char* get_apfs_features_string(apfs_superblock_t* apsb) {
    struct u64_string_mapping flags[] = {
        { APFS_FEATURE_DEFRAG_PRERELEASE,           "Reserved --- To avoid data corruption, this flag must not be set; this flag enabled a prerelease version of the defragmentation system in macOS 10.13 versions. Itʼs ignored by macOS 10.13.6 and later." },
        { APFS_FEATURE_HARDLINK_MAP_RECORDS,        "This volume has hardlink map records." },
        { APFS_FEATURE_DEFRAG,                      "Defragmentation is supported." },
        { APFS_FEATURE_STRICTATIME,                 "File access times are updated every time a file is read." },
        { APFS_FEATURE_VOLGRP_SYSTEM_INO_SPACE,     "This volume supports mounting a system and data volume as a single user-visible volume." },
    };
    
    // Initialise buffer as empty string
    const size_t bufsize = 2048;
    char* result_string = malloc(bufsize);
    if (!result_string) {
        fprintf(stderr, "\nERROR: %s: Couldn't create buffer `result_string`.\n", __func__);
        return NULL;
    }
    *result_string = '\0';

    size_t bytes_written = 0;
    for (size_t i = 0; i < ARRAY_SIZE(flags); i++) {
        if (apsb->apfs_features & flags[i].value) {
            bytes_written += snprintf(
                result_string + bytes_written,
                bufsize - bytes_written,
                "- %s\n",
                flags[i].string
            );

            if (bytes_written > bufsize - 1) {
                // Exhausted buffer; return early.
                fprintf(stderr, "\nERROR: %s: Buffer `result_string` too small for entire result.\n", __func__);
                return result_string;
            }
        }
    }

    if (bytes_written == 0) {
        // No flags set; use default string.
        snprintf(result_string, bufsize, "- No volume feature flags are set.\n");
    }

    // Truncate buffer
    result_string = realloc(result_string, strlen(result_string) + 1);

    return result_string;
}

/**
 * Get a human-readable string that lists the optional feature flags that are
 * set on a given APFS volume superblock.
 * 
 * apsb:    A pointer to the APFS volume superblock in question.
 * 
 * RETURN VALUE:
 *      A pointer to the first character of the string. The caller must free
 *      this pointer when it is no longer needed.
 */
char* get_apfs_readonly_compatible_features_string(apfs_superblock_t* apsb) {
    struct u64_string_mapping flags[] = {
        // empty
    };

    // Initialise buffer as empty string
    const size_t bufsize = 2048;
    char* result_string = malloc(bufsize);
    if (!result_string) {
        fprintf(stderr, "\nERROR: %s: Couldn't create buffer `result_string`.\n", __func__);
        return NULL;
    }
    *result_string = '\0';

    size_t bytes_written = 0;
    for (size_t i = 0; i < ARRAY_SIZE(flags); i++) {
        if (apsb->apfs_readonly_compatible_features & flags[i].value) {
            bytes_written += snprintf(
                result_string + bytes_written,
                bufsize - bytes_written,
                "- %s\n",
                flags[i].string
            );

            if (bytes_written > bufsize - 1) {
                // Exhausted buffer; return early.
                fprintf(stderr, "\nERROR: %s: Buffer `result_string` too small for entire result.\n", __func__);
                return result_string;
            }
        }
    }

    if (bytes_written == 0) {
        // No flags set; use default string.
        snprintf(result_string, bufsize, "- No read-only compatible volume feature flags are set.\n");
    }

    // Truncate buffer
    result_string = realloc(result_string, strlen(result_string) + 1);
    
    return result_string;
}

/**
 * Get a human-readable string that lists the backward-incompatible feature
 * flags that are set on a given APFS volume superblock.
 * 
 * apsb:    A pointer to the APFS volume superblock in question.
 * 
 * RETURN VALUE:
 *      A pointer to the first character of the string. The caller must free
 *      this pointer when it is no longer needed.
 */
char* get_apfs_incompatible_features_string(apfs_superblock_t* apsb) {
    struct u64_string_mapping flags[] = {
        { APFS_INCOMPAT_CASE_INSENSITIVE,           "Filenames on this volume are case-insensitive." },
        { APFS_INCOMPAT_DATALESS_SNAPS,             "At least one snapshot with no data exists for this volume." },
        { APFS_INCOMPAT_ENC_ROLLED,                 "This volume's encryption has changed keys at least once." },
        { APFS_INCOMPAT_NORMALIZATION_INSENSITIVE,  "Filenames on this volume are normalization insensitive." },
        { APFS_INCOMPAT_INCOMPLETE_RESTORE,         "This volume is being restored, or a restore operation to this volume was uncleanly aborted." },
        { APFS_INCOMPAT_SEALED_VOLUME,              "This volume is sealed (cannot be modified)." },
        { APFS_INCOMPAT_RESERVED_40,                "Reserved flag (0x40)." },
    };

    // Initialise buffer as empty string
    const size_t bufsize = 2048;
    char* result_string = malloc(bufsize);
    if (!result_string) {
        fprintf(stderr, "\nERROR: %s: Couldn't create buffer `result_string`.\n", __func__);
        return NULL;
    }
    *result_string = '\0';

    size_t bytes_written = 0;
    for (size_t i = 0; i < ARRAY_SIZE(flags); i++) {
        if (apsb->apfs_incompatible_features & flags[i].value) {
            bytes_written += snprintf(
                result_string + bytes_written,
                bufsize - bytes_written,
                "- %s\n",
                flags[i].string
            );

            if (bytes_written > bufsize - 1) {
                // Exhausted buffer; return early.
                fprintf(stderr, "\nERROR: %s: Buffer `result_string` too small for entire result.\n", __func__);
                return result_string;
            }
        }
    }

    if (bytes_written == 0) {
        // No flags set; use default string.
        snprintf(result_string, bufsize, "- No backward-incompatible volume feature flags are set.\n");
    }

    // Truncate buffer
    result_string = realloc(result_string, strlen(result_string) + 1);
    
    return result_string;
}

/**
 * Get a human-readable string that lists the volume flags that are
 * set on a given APFS volume superblock.
 * 
 * apsb:    A pointer to the APFS volume superblock in question.
 * 
 * RETURN VALUE:
 *      A pointer to the first character of the string. The caller must free
 *      this pointer when it is no longer needed.
 */
char* get_apfs_fs_flags_string(apfs_superblock_t* apsb) {
    // String to use if no flags are set
    char* default_string = "- No flags are set.\n";
    size_t default_string_len = strlen(default_string);
    
    const int NUM_FLAGS = 9;

    uint64_t flag_constants[] = {
        APFS_FS_UNENCRYPTED,
        APFS_FS_RESERVED_2,
        APFS_FS_RESERVED_4,
        APFS_FS_ONEKEY,
        APFS_FS_SPILLEDOVER,
        APFS_FS_RUN_SPILLOVER_CLEANER,
        APFS_FS_ALWAYS_CHECK_EXTENTREF,
        APFS_FS_RESERVED_80,
        APFS_FS_RESERVED_100,
    };
    
    char* flag_strings[] = {
        "Volume is unencrypted.",
        "Reserved flag (0x2).",
        "Reserved flag (0x4).",
        "Single VEK (volume encryption key) for all files in this volume.",
        "Volume has run out of allocated space on SSD, so has spilled over to other drives.",
        "Volume has spilled over and spillover cleaner must be run.",
        "When deciding whether to overwrite a file extent, always consult the extent reference tree.",
        "Reserved flag (0x80).",
        "Reserved flag (0x100).",
    };

    // Allocate sufficient memory for the result string
    size_t max_mem_required = 0;
    for (int i = 0; i < NUM_FLAGS; i++) {
        max_mem_required += strlen(flag_strings[i]) + 3;
        // `+ 3` accounts for prepending "- " and appending "\n" to each string
    }
    if (max_mem_required < default_string_len) {
        max_mem_required = default_string_len;
    }
    max_mem_required++; // Make room for terminating NULL byte

    char* result_string = malloc(max_mem_required);
    if (!result_string) {
        fprintf(stderr, "\nABORT: get_apfs_fs_flags_string: Could not allocate sufficient memory for `result_string`.\n");
        exit(-1);
    }

    char* cursor = result_string;

    // Go through possible flags, adding corresponding string to result if
    // that flag is set.
    for (int i = 0; i < NUM_FLAGS; i++) {
        if (apsb->apfs_fs_flags & flag_constants[i]) {
            *cursor++ = '-';
            *cursor++ = ' ';
            memcpy(cursor, flag_strings[i], strlen(flag_strings[i]));
            cursor += strlen(flag_strings[i]);
            *cursor++ = '\n';
        }
    }

    if (cursor == result_string) {
        // No strings were added, so it must be that no flags are set.
        memcpy(cursor, default_string, default_string_len);
        cursor += default_string_len;
    }

    *cursor = '\0';

    // Free up excess allocated memory.
    result_string = realloc(result_string, strlen(result_string) + 1);
    return result_string;
}

/**
 * Get a human-readable string describing the role of a given APFS volume
 * superblock.
 * 
 * apsb:    A pointer to the APFS volume superblock in question.
 * 
 * RETURN VALUE:
 *      A pointer to the first character of the string. The caller must free
 *      this pointer when it is no longer needed.
 */
char* get_apfs_role_string(apfs_superblock_t* apsb) {
    struct u64_string_mapping roles[] = {
        { APFS_VOL_ROLE_NONE,           "(no role)" },
        { APFS_VOL_ROLE_SYSTEM,         "System (contains a root directory for the system)" },
        { APFS_VOL_ROLE_USER,           "User (contains users' home directories)" },
        { APFS_VOL_ROLE_RECOVERY,       "Recovery (contains a recovery system)" },
        { APFS_VOL_ROLE_VM,             "Virtual memory (used as swap space for virtual memory)" },
        { APFS_VOL_ROLE_PREBOOT,        "Preboot (contains files needed to boot from an encrypted volumes)" },
        { APFS_VOL_ROLE_INSTALLER,      "Installer (used by the OS installer)" },
        { APFS_VOL_ROLE_DATA,           "Data (contains mutable data)" },
        { APFS_VOL_ROLE_BASEBAND,       "Baseband (used by the radio firmware)" },
        { APFS_VOL_ROLE_UPDATE,         "Update (used by the software update mechanism)" },
        { APFS_VOL_ROLE_XART,           "xART (used to manage OS access to secure user data" },
        { APFS_VOL_ROLE_HARDWARE,       "Hardware (used for firmware data)" },
        { APFS_VOL_ROLE_BACKUP,         "Backup (used by Time Machine to store backups)" },
        { APFS_VOL_ROLE_RESERVED_7,     "Reserved role 7 (Sidecar?) (role field value 0x1c0)" },
        { APFS_VOL_ROLE_RESERVED_8,     "Reserved role 8 (role field value 0x200)" },
        { APFS_VOL_ROLE_ENTERPRISE,     "Enterprise (used to store enterprise-managed data)" },
        { APFS_VOL_ROLE_RESERVED_10,    "Reserved role 10 (role field value 0x280)" },
        { APFS_VOL_ROLE_PRELOGIN,       "Pre-login (used to store system data used before login)" },
    };

    char* result_string = NULL;

    for (size_t i = 0; i < ARRAY_SIZE(roles); i++) {
        if (apsb->apfs_role == roles[i].value) {
            if (asprintf(&result_string, "%s", roles[i].string) == -1) {
                fprintf(stderr, "\nERROR: %s: Couldn't allocate sufficient memory for `result_string`.\n", __func__);
                return NULL;
            }
        }
        break;
    }
    
    if (!result_string) {
        // No role matched; use default string.
        if (asprintf(&result_string, "(unknown role) (role field value %#"PRIx16")", apsb->apfs_role) == -1) {
            fprintf(stderr, "%s: Couldn't allocate sufficient memory for `result_string` when returning default string.\n", __func__);
            return NULL;
        }
    }

    return result_string;
}

void print_apfs_modified_by(apfs_modified_by_t* data) {
    printf("- ID: %s\n", data->id);

    // Dividing timestamps by 10^9 to convert APFS timestamps (Unix timestamps
    // in nanoseconds) to Unix timestamps in seconds.
    // Trailing '\n' is provided by the result of `ctime()`.
    time_t timestamp = data->timestamp  / 1000000000;
    printf("- Timestamp: %s", ctime(&timestamp));

    printf("- Last XID: %"PRIx64"\n", data->last_xid);
}

/**
 * Print a nicely formatted string describing the data contained in a given
 * APFS volume superblock.
 * 
 * apsb:    A pointer to the APFS volume superblock in question.
 */
void print_apfs_superblock(apfs_superblock_t* apsb) {
    print_obj_phys(apsb);   // `apsb` equals `&(apsb->apfs_o)`
    printf("\n");

    char magic_string[] = {
        (char)(apsb->apfs_magic),
        (char)(apsb->apfs_magic >> 8),
        (char)(apsb->apfs_magic >> 16),
        (char)(apsb->apfs_magic >> 24),
        '\0'
    };
    printf("Magic string:                           %s\n",  magic_string);
    printf("Index within container volume array:    %u\n",  apsb->apfs_fs_index);
    printf("\n");

    printf("Volume name:        ### %s ###\n",  apsb->apfs_volname);
    
    char* tmp_string = get_apfs_role_string(apsb);
    printf("Role:               %s", tmp_string);
    free(tmp_string);
    
    printf("\n");

    tmp_string = get_apfs_fs_flags_string(apsb);
    printf("Flags:\n%s", tmp_string);
    free(tmp_string);

    tmp_string = get_apfs_features_string(apsb);
    printf("Supported features:\n%s", tmp_string);
    free(tmp_string);

    tmp_string = get_apfs_readonly_compatible_features_string(apsb);
    printf("Supported read-only compatible features:\n%s", tmp_string);
    free(tmp_string);

    tmp_string = get_apfs_incompatible_features_string(apsb);
    printf("Backward-incompatible features:\n%s", tmp_string);
    free(tmp_string);
    
    printf("\n");

    // Dividing timestamps by 10^9 to convert APFS timestamps (Unix timestamps
    // in nanoseconds) to Unix timestamps in seconds.
    // Trailing '\n' for each line is provided by the result of `ctime()`.
    time_t timestamp = apsb->apfs_unmount_time  / 1000000000;
    printf("Last unmount time:                  %s",    ctime(&timestamp));
    timestamp = apsb->apfs_last_mod_time / 1000000000;
    printf("Last modification time:             %s",    ctime(&timestamp));
    printf("\n");

    printf("Reserved blocks:                    %" PRIu64 " blocks\n", apsb->apfs_fs_reserve_block_count);
    printf("Block quota:                        %" PRIu64 " blocks\n", apsb->apfs_fs_quota_block_count);
    printf("Allocated blocks:                   %" PRIu64 " blocks\n", apsb->apfs_fs_alloc_count);
    printf("\n");

    printf("Volume object map Physical OID:     0x%" PRIx64 "\n",    apsb->apfs_omap_oid);
    printf("\n");
    
    printf("Root tree info:\n");
    printf("- OID:              0x%" PRIx64 "\n",  apsb->apfs_root_tree_oid);
    printf("- Storage type:     %s\n",      o_storage_type_to_string(apsb->apfs_root_tree_type));
    
    tmp_string = get_o_type_flags_string(apsb->apfs_root_tree_type);
    printf("- Type flags:       %s\n",  tmp_string);
    free(tmp_string);

    tmp_string = get_o_type_string(apsb->apfs_root_tree_type);
    printf("- Object type:      %s\n",  tmp_string);
    free(tmp_string);

    printf("\n");

    printf("Extent-reference tree info:\n");
    printf("- OID:              0x%" PRIx64 "\n",  apsb->apfs_extentref_tree_oid);
    printf("- Storage type:     %s\n",      o_storage_type_to_string(apsb->apfs_extentref_tree_type));
    
    tmp_string = get_o_type_flags_string(apsb->apfs_extentref_tree_type);
    printf("- Type flags:       %s\n",  tmp_string);
    free(tmp_string);

    tmp_string = get_o_type_string(apsb->apfs_extentref_tree_type);
    printf("- Object type:      %s\n",  tmp_string);
    free(tmp_string);

    printf("\n");

    printf("Snapshot metadata tree info:\n");
    printf("- OID:              0x%" PRIx64 "\n",  apsb->apfs_snap_meta_tree_oid);
    printf("- Storage type:     %s\n",      o_storage_type_to_string(apsb->apfs_snap_meta_tree_type));
    
    tmp_string = get_o_type_flags_string(apsb->apfs_snap_meta_tree_type);
    printf("- Type flags:       %s\n",  tmp_string);
    free(tmp_string);

    tmp_string = get_o_type_string(apsb->apfs_snap_meta_tree_type);
    printf("- Object type:      %s\n",  tmp_string);
    free(tmp_string);

    printf("\n");

    printf("On next mount, revert to:\n");
    printf("- snapshot with this XID:                           0x%" PRIx64 "\n",  apsb->apfs_revert_to_xid);
    printf("- APFS volume superblock with this Physical OID:    0x%" PRIx64 "\n",  apsb->apfs_revert_to_sblock_oid);
    printf("\n");

    printf("Next file-system object ID that will be assigned:   0x%" PRIx64 "\n",  apsb->apfs_next_obj_id);
    printf("Next document ID that will be assigned:             0x%x\n",    apsb->apfs_next_doc_id);
    printf("\n");

    printf("Number of:\n");
    printf("\n");
    printf("- regular files:                %" PRIu64 "\n",    apsb->apfs_num_files);
    printf("- directories:                  %" PRIu64 "\n",    apsb->apfs_num_directories);
    printf("- symbolic links:               %" PRIu64 "\n",    apsb->apfs_num_symlinks);
    printf("- other file-system objects:    %" PRIu64 "\n",    apsb->apfs_num_other_fsobjects);
    printf("\n");
    printf("- snapshots:                    %" PRIu64 "\n",    apsb->apfs_num_snapshots);
    printf("- block allocations ever made:  %" PRIu64 "\n",    apsb->apfs_total_block_alloced);
    printf("- block liberations ever made:  %" PRIu64 "\n",    apsb->apfs_total_blocks_freed);
    printf("\n");

    printf("UUID:   0x%016" PRIx64 "%016" PRIx64 "\n",
        *((uint64_t*)(apsb->apfs_vol_uuid) + 1),
        * (uint64_t*)(apsb->apfs_vol_uuid)
    );

    printf("Formatted by:\n");
    print_apfs_modified_by(&(apsb->apfs_formatted_by));
    printf("\n");

    printf("Last modified by:\n");
    print_apfs_modified_by(&(apsb->apfs_modified_by));

    /*
     * TODO: Print the following fields:
     * - apfs_root_to_xid
     * - apfs_er_state_oid
     * - apfs_cloneinfo_id_epoch
     * - apfs_cloneinfo_xid
     * - apfs_snap_meta_ext_oid
     * - apfs_volume_group_id
     * - apfs_integrity_meta_oid
     * - apfs_fext_tree_oid
     * - apfs_fext_tree_type
     */
}
