function(add_objects_begin)
    message("-----------------------------------------------------------------------------\n")
endfunction()


function(add_objects_end)
    message("-----------------------------------------------------------------------------\n")
endfunction()


function(add_object _name _directory _enabled _condition _description)
    set(_text "[Object] ")

    if(NOT ${_enabled})
        set(_text "${_text} [ ] ${_name}")
    elseif (${_condition})
        set(_text "${_text} [V] ${_name}")
        add_subdirectory(${_directory})
    else (${_condition})
        set(_text "${_text} [X] ${_name}")
    endif(NOT ${_enabled})

    set(_text "${_text} \n          ${_description}.")
    message(${_text})
endfunction()