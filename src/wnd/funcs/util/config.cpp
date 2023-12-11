#include "config.h"

bool menu_style_toggle[ OBJ_BASE_COUNT / 2 ];

bool init_config() {
  char *doc_path = nullptr;
  size_t doc_len = 0;
  _dupenv_s( &doc_path, &doc_len, "USERPROFILE" );

  if( !doc_path )
    return false;

  size_t edit_path_len = doc_len + strlen( "\\Documents\\edit\\" ) + 1;
  char *edit_path = new char[ edit_path_len ];
  strcpy_s( edit_path, edit_path_len, doc_path );
  strcat_s( edit_path, edit_path_len, "\\Documents\\edit\\" );

  if( !CreateDirectoryA( edit_path, NULL ) ) {
    if( GetLastError() != ERROR_ALREADY_EXISTS ) {
      free( doc_path );
      delete[] edit_path;
      return false;
    }
  }

  edit_path_len += strlen( "edit.cfg" );
  char *cfg_path = new char[ edit_path_len ];
  strcpy_s( cfg_path, edit_path_len, edit_path );
  strcat_s( cfg_path, edit_path_len, "edit.cfg" );

  if( !cfg_path ) {
    free( doc_path );
    delete[] cfg_path;
    delete[] edit_path;
    return false;
  }

  HANDLE cfg_file = CreateFileA( cfg_path,
    GENERIC_READ | GENERIC_WRITE, 0,
    NULL, OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL, NULL
  );

  if( GetLastError() == ERROR_ALREADY_EXISTS ) {
    free( doc_path );
    delete[] cfg_path;
    delete[] edit_path;
    return true;
  } else {
    char default_cfg = 0x11100000;
    ptr bytes_written;
    WriteFile( cfg_file, &default_cfg, 1, &bytes_written, NULL );
    CloseHandle( cfg_file );

    for( s32 i = 0; i < 3; i++ )
      menu_style_toggle[ i ] = true;
  }

  free( doc_path );
  delete[] cfg_path;
  delete[] edit_path;
  return true;
}