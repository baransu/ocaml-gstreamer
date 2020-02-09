open Configurator.V1.Flags;
module C = Configurator.V1;

type os =
  | Windows
  | Mac
  | Linux
  | Unknown;

let uname = () => {
  let ic = Unix.open_process_in("uname");
  let uname = input_line(ic);
  let () = close_in(ic);
  uname;
};

let get_os =
  switch (Sys.os_type) {
  | "Win32" => Windows
  | _ =>
    switch (uname()) {
    | "Darwin" => Mac
    | "Linux" => Linux
    | _ => Unknown
    }
  };

let () = {
  let ccopt = s => ["-ccopt", s];
  let cclib = s => ["-cclib", s];

  let flags =
    switch (get_os) {
    | Linux =>
      []
      @ cclib("-lgstreamer-1.0")
      @ cclib("-lgobject-2.0")
      @ cclib("-lgstapp-1.0")
      @ cclib("-lgstbase-1.0")
      // @ cclib("-lintl")
      @ ccopt("-I/usr/include")

    | Windows => []

    | _ => []
    };

  let cflags =
    switch (get_os) {
    | Mac => []

    | Linux => []

    | Windows => []

    | _ => failwith("cflags: unknown platform")
    };

  let libs =
    switch (get_os) {
    | Mac =>
      []
      @ ["-lgstreamer-1.0"]
      @ ["-lgobject-2.0"]
      @ ["-lgstapp-1.0"]
      @ ["-lgstbase-1.0"]
    // @ ["-lintl"]

    | Linux =>
      []
      @ ["-lgstreamer-1.0"]
      @ ["-lgobject-2.0"]
      @ ["-lgstapp-1.0"]
      @ ["-lgstbase-1.0"]
    // @ ["-lintl"]

    | Windows => []

    | _ => failwith("libs: Unknown platform")
    };

  write_sexp("flags.sexp", flags);
  write_lines("c_flags.txt", cflags);
  write_sexp("c_flags.sexp", cflags);
  write_sexp("c_library_flags.sexp", libs);
  write_lines("c_library_flags.txt", libs);
  write_sexp(
    "cclib_c_library_flags.sexp",
    libs |> List.map(o => ["-cclib", o]) |> List.flatten,
  );
};
