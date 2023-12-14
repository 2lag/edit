#include "config.h"

bool menu_style_toggle[ OBJ_BASE_COUNT / 2 ];

void load_config() {
  // open file
  // read first 3 bits & set to menu_style_toggle
  //  - test print bits first bc encoding may b fucky
  // close file

  return;
}

void init_config() {
  for( s32 i = 0; i < 3; i++ )
      menu_style_toggle[ i ] = true;

  char *doc_path = nullptr;
  size_t doc_len = 0;
  _dupenv_s( &doc_path, &doc_len, "USERPROFILE" );

  if( !doc_path ) {
#ifdef _DEBUG
    printf("failed @ docpath\n");
#endif
    return;
  }

  size_t edit_path_len = doc_len + strlen("\\Documents\\edit\\") + 1;
  char *edit_path = new char[ edit_path_len ];
  strcpy_s( edit_path, edit_path_len, doc_path );
  strcat_s( edit_path, edit_path_len, "\\Documents\\edit\\" );

  if( !CreateDirectoryA( edit_path, NULL ) ) {
    if( GetLastError() != ERROR_ALREADY_EXISTS ) {
      free( doc_path );
      delete[] edit_path;
#ifdef _DEBUG
      printf("failed @ directory\n");
#endif
      return;
    }
  }

  edit_path_len += strlen("edit.cfg");
  char *cfg_path = new char[ edit_path_len ];
  strcpy_s( cfg_path, edit_path_len, edit_path );
  strcat_s( cfg_path, edit_path_len, "edit.cfg" );

  if( !cfg_path ) {
    free( doc_path );
    delete[] cfg_path;
    delete[] edit_path;
#ifdef _DEBUG
      printf("failed @ cfg path\n");
#endif
    return ;
  }

  HANDLE cfg_file = CreateFileA( cfg_path,
    GENERIC_READ | GENERIC_WRITE, 0,
    NULL, OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL, NULL
  );

  if( GetLastError() == ERROR_ALREADY_EXISTS ) {
#ifdef _DEBUG
      printf("loading saved config\n");
#endif
    load_config();
  } else {
#ifdef _DEBUG
      printf( "writing config to %s\n", cfg_path );
#endif
    u64 default_cfg = 0x11100000;
    ptr bytes_written;
    WriteFile( cfg_file, &default_cfg, 1, &bytes_written, NULL );
    CloseHandle( cfg_file );
  }

  free( doc_path );
  delete[] cfg_path;
  delete[] edit_path;
  return;
}

bool toggle_config_idx( s32 idx ) {
  
  
  return true;
}