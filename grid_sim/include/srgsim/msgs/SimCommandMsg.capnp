@0xcbaac9bc054f0dc5;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srgsim");
using IDMsg = import "/capnzero/ID.capnp";

struct SimCommandMsg {
  senderID @0 :IDMsg.ID;
  action @1 :Action;
  objectID @2 :IDMsg.ID;
  x @3: UInt32;
  y @4: UInt32;

  enum Action {
      spawn @0;
      goleft @1;
      goright @2;
      goup @3;
      godown @4;
      pickup @5;
      putdown @6;
      open @7;
      close @8;
  }
}