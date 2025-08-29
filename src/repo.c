#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "repo.h"
#include "util.h"
#ifndef LEGIT_DIR_NAME
#define LEGIT_DIR_NAME ".legit"
#endif

static int write_text_atomic(const char *tmpdir, const char *dest, const char *text){
  const void *data = text ? (const void *)text : (const void * )"";
  size_t len = text ? strlen(text) : 0;
  return xwritefile_atomic(tmpdir, dest, data,len);
}

int repo_init(const char *workdir, legit_init_state *state_out,
              char *abs_out, size_t abs_out_sz){

  if(!xrealpath(workdir, abs_out, abs_out_sz)){
    fprintf(stderr, "error: cannot resolve path: '%s': %s\n", workdir, strerror(errno));
    return 1;
  }

  char legitdir[PATH_MAX];
  char objects[PATH_MAX], refs[PATH_MAX], heads[PATH_MAX], tags[PATH_MAX], tmpd[PATH_MAX];
  char head_path[PATH_MAX], index_path[PATH_MAX], desc_path[PATH_MAX];
if (xpath_join(legitdir, sizeof(legitdir), abs_out, LEGIT_DIR_NAME)) return 1;
  if(xpath_join(objects, sizeof(objects), legitdir, "objects")) return 1;

    if (xpath_join(refs,    sizeof(refs),    legitdir,   "refs"))    return 1;
    if (xpath_join(heads,   sizeof(heads),   refs,    "heads"))   return 1;
    if (xpath_join(tags,    sizeof(tags),    refs,    "tags"))    return 1;
    if (xpath_join(tmpd,    sizeof(tmpd),    legitdir,   "tmp"))     return 1;

    if (xpath_join(head_path,  sizeof(head_path),  legitdir, "HEAD"))        return 1;
    if (xpath_join(index_path, sizeof(index_path), legitdir, "index"))       return 1;
    if (xpath_join(desc_path,  sizeof(desc_path),  legitdir, "description")) return 1;



  if(xexists_dir(legitdir)){
    if(!xexists_file(head_path)){
      fprintf(stderr, "error: existing repository at '%s' is malformed (missing HEAD)\n", legitdir);
      return 1;
    }
    if(!xexists_dir(objects) || !xexists_dir(refs) || !xexists_dir(heads) || !xexists_dir(tags)){
      fprintf(stderr, "error: existing repository at '%s' is malformed (missing core dirs)\n", legitdir);
      return 1;
    }

    if(state_out) *state_out = LEGIT_INIT_REINIT;
    return 0;
  }

  if(xexists_file(legitdir)){
    fprintf(stderr, "error: path '.legit' exists and is not a directory\n");

    return 1;
  }

  if(xmkdir(legitdir , 0755)) {xperrorf("cannot create directory", legitdir); return 1;}

    if (xmkdir(objects, 0755)) { xperrorf("cannot create directory", objects); return 1; }
    if (xmkdir(refs,    0755)) { xperrorf("cannot create directory", refs); return 1; }
    if (xmkdir(heads,   0755)) { xperrorf("cannot create directory", heads); return 1; }
    if (xmkdir(tags,    0755)) { xperrorf("cannot create directory", tags); return 1; }


  (void) xmkdir(tmpd, 0700);

  const char *head_text = "ref: refs/heads/master\n";
  if(write_text_atomic(tmpd, head_path, head_text)){
    xperrorf("cannot write HEAD", head_path);
    return 1;
  }
  if(xwritefile_atomic(tmpd, index_path, NULL, 0)){
    xperrorf("cannot write index", index_path);
    return 1;

  }

  const char *desc_text = "Unname LeGit repository; edit this file 'descripition' to name it.\n";

  if(write_text_atomic(tmpd, desc_path, desc_text)){
    xperrorf("cannot write descripition", desc_path);
    return 1;
  }

  if(!xexists_file(head_path) || !xexists_dir(objects) || !xexists_dir(heads) || !xexists_dir(tags)){
    fprintf(stderr, "error: repository initialization incomplete at '%s'\n", legitdir)
    ;

    return 1;
  }

  if(state_out) *state_out = LEGIT_INIT_FRESH;
  return 0;


}





