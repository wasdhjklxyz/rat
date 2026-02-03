{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    systems.url = "github:nix-systems/default";
  };

  outputs = { self, nixpkgs, systems }:
    let
      eachSystem = nixpkgs.lib.genAttrs (import systems);
    in {
      devShells = eachSystem (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in {
          default = pkgs.mkShell {
            env = {
              LAB_BRIDGE = "lab0";
              LAB_DISK_DIR = "./disks";
              LAB_GATEWAY = "10.10.10.1";
              LAB_NETCIDR = "10.10.10.0/24";
              LAB_TARGET_IP = "10.10.10.10";
              LAB_OPERATOR_IP = "10.10.10.20";
              LAB_TARGET_MAC = "52:54:00:00:00:10";
              LAB_OPERATOR_MAC = "52:54:00:00:00:20";
              LAB_USER = "rat";
              LAB_PASS = "rat";
              LAB_DOCKER_IMG = "rockylinux/rockylinux:10";
              LAB_DOCKER_IMG_NAME = "rockylinux10-dev";
            };
            packages = with pkgs; [
              qemu
              iptables
              cdrkit
              openssl_oqs
              docker
              (writeShellScriptBin "mkdisks" (builtins.readFile ./scripts/mkdisks.sh))
              (writeShellScriptBin "mkseed" (builtins.readFile ./scripts/mkseed.sh))
              (writeShellScriptBin "net" (builtins.readFile ./scripts/net.sh))
              (writeShellScriptBin "target" (builtins.readFile ./scripts/target.sh))
              (writeShellScriptBin "operator" (builtins.readFile ./scripts/operator.sh))
              (writeShellScriptBin "build" (builtins.readFile ./scripts/build.sh))
              (writeShellScriptBin "mkdocker" (builtins.readFile ./scripts/mkdocker.sh))
              (writeShellScriptBin "deploy" (builtins.readFile ./scripts/deploy.sh))
            ];
          };
        });
    };
}
