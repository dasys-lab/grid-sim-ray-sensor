@0xcbaac9bc054f0dc5;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("srgsim");
using IDMsg = import "/capnzero/ID.capnp";
# TODO container and msg and send and receive in WM
struct SimSensorMsg {
  receiverID @0 :IDMsg.ID;
  action @1 :List(Perception);
  x @2: UInt32;
  y @3: UInt32;

  enum Perception {
      location @0;
      object @1;
  }
}