cmake_minimum_required(VERSION 3.15)



function(get_commit_isdirty)

    execute_process(COMMAND
        "${GIT_EXECUTABLE}" status --short --untracked-files=no
        WORKING_DIRECTORY "${WORK_DIR}"
        OUTPUT_VARIABLE COMMIT_ISDIRTY_OUT
        RESULT_VARIABLE exit_code
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(exit_code EQUAL 0 AND "${COMMIT_ISDIRTY_OUT}" STREQUAL "")
        set(COMMIT_ISDIRTY 0 PARENT_SCOPE)
    else()
        set(COMMIT_ISDIRTY 1 PARENT_SCOPE)
    endif()

endfunction()



function(get_commit_hash)

    execute_process(COMMAND
        "${GIT_EXECUTABLE}" rev-parse --short=12 HEAD
        WORKING_DIRECTORY "${WORK_DIR}"
        OUTPUT_VARIABLE COMMIT_HASH_VAR
        RESULT_VARIABLE exit_code
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(exit_code EQUAL 0)
        set(COMMIT_HASH ${COMMIT_HASH_VAR} PARENT_SCOPE)
    else()
        set(COMMIT_HASH unknown PARENT_SCOPE)
    endif()

endfunction()



function(get_patch_version)


    if(${DAEMON_PATCH_VERSION} STREQUAL patch)

        execute_process(COMMAND
            "${GIT_EXECUTABLE}" describe --tags --first-parent --abbrev=0
            WORKING_DIRECTORY "${WORK_DIR}"
            OUTPUT_VARIABLE LAST_TAG
            RESULT_VARIABLE exit_code
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        execute_process(COMMAND
            "${GIT_EXECUTABLE}" rev-list HEAD...${LAST_TAG} --count
            WORKING_DIRECTORY "${WORK_DIR}"
            OUTPUT_VARIABLE NUMBER_PATCH
            RESULT_VARIABLE exit_code
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        if(exit_code EQUAL 0)
            set(DAEMON_PATCH_VERSION ${NUMBER_PATCH} PARENT_SCOPE)
        else()
            set(DAEMON_PATCH_VERSION 0 PARENT_SCOPE)
        endif()
    endif()

endfunction()



find_package(Git QUIET)
if(Git_FOUND)

    get_commit_isdirty()
    get_commit_hash()
    get_patch_version()

else()

    if(${DAEMON_PATCH_VERSION} STREQUAL patch)
        set(DAEMON_PATCH_VERSION 0)
    endif()

    set(COMMIT_ISDIRTY       1)
    set(COMMIT_HASH    unknown)

endif()


configure_file(${IN_FILE} ${OUT_FILE} @ONLY)
