open TestFramework;
open Vim;

let resetBuffer = () => Helpers.resetBuffer("test/testfile.txt");

describe("Quit", ({test, _}) => {
  test("q command", ({expect}) => {
    let b = resetBuffer();

    let updates = ref([]);

    let dispose =
      onQuit((quitType, forced) =>
        updates := [(quitType, forced), ...updates^]
      );

    command("q");
    let (qt, forced) = List.hd(updates^);

    expect.int(List.length(updates^)).toBe(1);

    switch (qt) {
    | QuitOne(buf) => expect.bool(buf == b).toBe(true)
    | _ => expect.string("Should've been QuitOne").toEqual("")
    };

    expect.bool(forced).toBe(false);

    dispose();
  });
  test("q input", ({expect}) => {
    let b = resetBuffer();

    let updates = ref([]);

    let dispose =
      onQuit((quitType, forced) =>
        updates := [(quitType, forced), ...updates^]
      );

    input(":");
    input("q");
    input("<cr>");
    let (qt, forced) = List.hd(updates^);

    expect.int(List.length(updates^)).toBe(1);

    switch (qt) {
    | QuitOne(buf) => expect.bool(buf == b).toBe(true)
    | _ => expect.string("Should've been QuitOne").toEqual("")
    };

    expect.bool(forced).toBe(false);

    dispose();
  });
  test("q!", ({expect}) => {
    let b = resetBuffer();

    let updates = ref([]);

    let dispose =
      onQuit((quitType, forced) =>
        updates := [(quitType, forced), ...updates^]
      );

    command("q!");
    let (qt, forced) = List.hd(updates^);

    expect.int(List.length(updates^)).toBe(1);

    switch (qt) {
    | QuitOne(buf) => expect.bool(buf == b).toBe(true)
    | _ => expect.string("Should've been QuitOne").toEqual("")
    };

    expect.bool(forced).toBe(true);

    dispose();
  });
  test("qall", ({expect}) => {
    let _ = resetBuffer();

    let updates = ref([]);

    let dispose =
      onQuit((quitType, forced) =>
        updates := [(quitType, forced), ...updates^]
      );

    command("qall");
    let (qt, forced) = List.hd(updates^);

    expect.int(List.length(updates^)).toBe(1);

    switch (qt) {
    | QuitAll => expect.bool(true).toBe(true)
    | _ => expect.string("Should've been QuitAll").toEqual("")
    };

    expect.bool(forced).toBe(false);

    dispose();
  });
});
