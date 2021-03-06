open Types;

type autocmdListener = (autocmd, Native.buffer) => unit;
type bufferListener = Native.buffer => unit;
type bufferModifiedChangedListener = (int, bool) => unit;
type bufferFilenameChangedListener = BufferMetadata.t => unit;
type bufferUpdateListener = BufferUpdate.t => unit;
type commandLineUpdateListener = Types.cmdline => unit;
type directoryChangedListener = string => unit;
type cursorMovedListener = Position.t => unit;
type leftColumnChangedListener = int => unit;
type messageListener = (Types.msgPriority, string, string) => unit;
type modeChangedListener = mode => unit;
type quitListener = (Types.quitType, bool) => unit;
type topLineChangedListener = int => unit;
type visualRangeChangedListener = VisualRange.t => unit;
type windowMovementListener = (Types.windowMovementType, int) => unit;
type windowSplitListener = (Types.windowSplitType, string) => unit;
type yankListener = Yank.t => unit;
type noopListener = unit => unit;

let autocmd: ref(list(autocmdListener)) = ref([]);
let bufferEnter: ref(list(bufferListener)) = ref([]);
let bufferFilenameChanged: ref(list(bufferFilenameChangedListener)) =
  ref([]);
let bufferModifiedChanged: ref(list(bufferModifiedChangedListener)) =
  ref([]);
let bufferUpdate: ref(list(bufferUpdateListener)) = ref([]);
let bufferLeave: ref(list(bufferListener)) = ref([]);
let commandLineEnter: ref(list(commandLineUpdateListener)) = ref([]);
let commandLineUpdate: ref(list(commandLineUpdateListener)) = ref([]);
let commandLineLeave: ref(list(noopListener)) = ref([]);
let directoryChanged: ref(list(directoryChangedListener)) = ref([]);
let cursorMoved: ref(list(cursorMovedListener)) = ref([]);
let leftColumnChanged: ref(list(leftColumnChangedListener)) = ref([]);
let message: ref(list(messageListener)) = ref([]);
let modeChanged: ref(list(modeChangedListener)) = ref([]);
let quit: ref(list(quitListener)) = ref([]);
let stopSearchHighlight: ref(list(noopListener)) = ref([]);
let topLineChanged: ref(list(topLineChangedListener)) = ref([]);
let unhandledEscape: ref(list(noopListener)) = ref([]);
let visualRangeChanged: ref(list(visualRangeChangedListener)) = ref([]);
let windowMovement: ref(list(windowMovementListener)) = ref([]);
let windowSplit: ref(list(windowSplitListener)) = ref([]);
let yank: ref(list(yankListener)) = ref([]);
