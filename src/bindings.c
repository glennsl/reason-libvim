#include "libvim.h"

#include <caml/alloc.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>

#define Val_none Val_int(0)

static value Val_some(value v) {
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 0);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

void onBufferChanged(bufferUpdate_T bu) {
  static value *lv_onBufferChanged = NULL;

  if (lv_onBufferChanged == NULL) {
    lv_onBufferChanged = caml_named_value("lv_onBufferChanged");
  }

  value *pArgs = (value *)malloc(sizeof(value) * 4);
  pArgs[0] = (value)bu.buf;
  pArgs[1] = Val_int(bu.lnum);
  pArgs[2] = Val_int(bu.lnume);
  pArgs[3] = Val_long(bu.xtra);

  caml_callbackN(*lv_onBufferChanged, 4, pArgs);

  free(pArgs);
}

void onAutocommand(event_T event, buf_T *buf) {
  static value *lv_onAutocmd = NULL;

  if (lv_onAutocmd == NULL) {
    lv_onAutocmd = caml_named_value("lv_onAutocommand");
  }

  caml_callback2(*lv_onAutocmd, Val_int(event), (value)buf);
}

CAMLprim value libvim_vimInit(value unit) {
  vimSetBufferUpdateCallback(&onBufferChanged);
  vimSetAutoCommandCallback(&onAutocommand);

  char *args[0];
  vimInit(0, args);
  return Val_unit;
}

CAMLprim value libvim_vimInput(value v) {
  char_u *s;
  s = (char_u *)String_val(v);
  vimInput(s);
  return Val_unit;
}

CAMLprim value libvim_vimCommand(value v) {
  char_u *s;
  s = (char_u *)String_val(v);
  vimExecute(s);
  return Val_unit;
}

CAMLprim value libvim_vimGetMode(value unit) {
  int mode = vimGetMode();

  printf("RAW MODE: %d\n", mode);
  printf("get_real_state: %d\n", get_real_state());

  int val = 0;

  if ((mode & INSERT) == INSERT) {
    val = 1;
  } else if ((mode & CMDLINE) == CMDLINE) {
    val = 2;
  } else if ((mode & REPLACE_FLAG) == REPLACE_FLAG) {
    val = 3;
  } else if ((mode & VISUAL) == VISUAL) {
    val = 4;
  } else if ((mode & OP_PENDING) == OP_PENDING) {
    val = 5;
  }

  return Val_int(val);
}

CAMLprim value libvim_vimBufferGetId(value v) {
  buf_T *buf = (buf_T *)v;
  printf("vimBufferGetId...\n");
  int id = vimBufferGetId(buf);
  printf("vimBufferGetId...DONE\n");
  return Val_int(id);
}

CAMLprim value libvim_vimBufferOpen(value v) {
  CAMLparam1(v);
  char_u *s;
  s = (char_u *)String_val(v);
  buf_T *buf = vimBufferOpen(s, 0, 1);
  value vbuf = (value)buf;
  CAMLreturn(vbuf);
}

CAMLprim value libvim_vimBufferGetById(value v) {
  CAMLparam1(v);
  CAMLlocal1(ret);
  buf_T *buf = vimBufferGetById(Int_val(v));

  if (!buf) {
    ret = Val_none;
  } else {
    value b = (value)buf;
    ret = Val_some(b);
  }

  CAMLreturn(ret);
}

CAMLprim value libvim_vimBufferGetFilename(value v) {
  CAMLparam1(v);
  CAMLlocal1(ret);
  buf_T *buf = (buf_T *)v;

  if (buf == NULL) {
    printf("libvim_vimBufferGetFilename - buffer is NULL");
    ret = Val_none;
  } else {
    char_u *fname = vimBufferGetFilename(buf);
    if (fname == NULL) {
      printf("libvim_vimBufferGetFilename - fname is NULL");
      ret = Val_none;
    } else {
      printf("libvim_vimBufferGetFilename - fname is %s\n", fname);
      ret = Val_some(caml_copy_string(fname));
    }
  }

  CAMLreturn(ret);
}

CAMLprim value libvim_vimBufferGetModified(value v) {
  buf_T *buf = (buf_T *)v;

  if (vimBufferGetModified(buf) == TRUE) {
    return Val_true;
  } else {
    return Val_false;
  }
}

CAMLprim value libvim_vimBufferGetChangedTick(value v) {
  buf_T *buf = (buf_T *)v;

  long tick = vimBufferGetLastChangedTick(buf);
  return Long_val(tick);
}

CAMLprim value libvim_vimBufferGetFiletype(value v) {
  CAMLparam1(v);
  CAMLlocal1(ret);
  buf_T *buf = (buf_T *)v;

  if (buf == NULL) {
    ret = Val_none;
  } else {
    char_u *ftype = vimBufferGetFiletype(buf);
    if (ftype == NULL) {
      ret = Val_none;
    } else {
      ret = Val_some(caml_copy_string(ftype));
    }
  }

  CAMLreturn(ret);
}

CAMLprim value libvim_vimBufferGetCurrent(value unit) {
  return (value)vimBufferGetCurrent();
}

CAMLprim value libvim_vimBufferGetLineCount(value v) {
  buf_T *buf = (buf_T *)v;
  size_t count = vimBufferGetLineCount(buf);
  return Val_long(count);
}

CAMLprim value libvim_vimBufferGetLine(value vBuf, value vLine) {
  CAMLparam2(vBuf, vLine);
  CAMLlocal1(ret);
  buf_T *buf = (buf_T *)vBuf;
  int line = Int_val(vLine);

  char_u *c = vimBufferGetLine(buf, line);
  ret = caml_copy_string(c);
  CAMLreturn(ret);
}

CAMLprim value libvim_vimBufferSetCurrent(value v) {
  buf_T *buf = (buf_T *)v;

  vimBufferSetCurrent(buf);
  return Val_unit;
}

CAMLprim value libvim_vimCursorGetLine(value unit) {
  int line = vimCursorGetLine();
  return Val_int(line);
}

CAMLprim value libvim_vimCursorGetColumn(value unit) {
  int column = vimCursorGetColumn();
  return Val_int(column);
}