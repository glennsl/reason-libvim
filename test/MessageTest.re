open TestFramework;
open Vim;

let reset = () => Helpers.resetBuffer("test/testfile.txt");

describe("Messages", ({test, _}) => {
  test("echo dispatches message", ({expect}) => {
    let _ = reset();

    let messages = ref([]);
    let dispose =
      onMessage((priority, title, contents) =>
        messages := [(priority, title, contents), ...messages^]
      );

    command("echo 'hello'");

    expect.int(List.length(messages^)).toBe(1);

    let (priority, title, contents) = List.hd(messages^);

    expect.string(title).toEqual("");
    expect.string(contents).toEqual("hello");
    expect.bool(priority == Types.Info).toBe(true);

    dispose();
  });
  test("echoerr dispatches error message", ({expect}) => {
    let _ = reset();

    let messages = ref([]);
    let dispose =
      onMessage((priority, title, contents) =>
        messages := [(priority, title, contents), ...messages^]
      );

    command("echoerr 'aproblem'");

    /* TODO: Fix this! */
    /* expect.int(List.length(messages^)).toBe(1); */

    let (priority, title, contents) = List.hd(messages^);

    expect.string(title).toEqual("");
    expect.string(contents).toEqual("aproblem");
    expect.bool(priority == Types.Error).toBe(true);

    dispose();
  });
});
