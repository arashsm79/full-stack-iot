{
  description = "Adobe Connecter  Flake";

  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    pypi-deps-db = {
      url = "github:DavHau/pypi-deps-db";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.mach-nix.follows = "mach-nix";
    };
    mach-nix = {
      url = "github:DavHau/mach-nix";
      inputs.nixpkgs.follows = "nixpkgs";
      inputs.flake-utils.follows = "flake-utils";
      inputs.pypi-deps-db.follows = "pypi-deps-db";
    };
  };

  outputs = { self, nixpkgs, flake-utils, mach-nix, pypi-deps-db }:
    (flake-utils.lib.eachSystem [ "x86_64-linux" ] (system:
      let
        pkgs = import nixpkgs {
          inherit system;
          config.allowUnfree = true;
        };
        python = "python39";
        mach-nix-wrapper = import mach-nix {
          inherit pkgs python;
          pypiDataRev = pypi-deps-db.rev;
          pypiDataSha256 = pypi-deps-db.narHash;
        };

        requirements = ''
          django-extensions
          werkzeug
          django
          pyOpenSSL
        '';
        pythonShell = mach-nix-wrapper.mkPython {
          inherit requirements;
          providers = {
            _default = "wheel,sdist,nixpkgs";
          };
        };

      in
      {
        devShell = pkgs.mkShell {
          buildInputs = [
            pythonShell
          ];
          shellHook = ''
            export PYTHONPATH="${pythonShell}/${pythonShell.python.sitePackages}:$PYTHONPATH";
          '';
        };
      })
    ) // {
      overlay = final: prev: { };
    };
}
