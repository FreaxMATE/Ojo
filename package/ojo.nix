{
    lib,
    stdenv,
    gettext,
    pkg-config,
    gtkmm4,
    libxml2,
    bash,
    gtk4,
    gtk3,
    libadwaita,
    glib,
    wrapGAppsHook4,
    meson,
    libvlc,
    ninja,
    gsettings-desktop-schemas,
    itstool,
    librsvg,
    gdk-pixbuf,
    libgtop,
    python3,
    systemd,
}:

stdenv.mkDerivation (finalAttrs: {
    pname = "ojo";
    version = "1.0.0";

    src = builtins.path { path = /home/kunruh/src/Ojo ; name = "Ojo"; };

    patches = [
        #./ojo-fix-paths.patch
    ];

    nativeBuildInputs = [
        pkg-config
        gettext
        itstool
        wrapGAppsHook4
        meson
        ninja
        glib
    ];

    buildInputs = [
        bash
        gtk4
        gtk3
        libadwaita
        python3
        libvlc
        glib
        libxml2
        gtkmm4
        libgtop
        gdk-pixbuf
        librsvg
        gsettings-desktop-schemas
        systemd
    ];

    doCheck = true;

    passthru = {
        updateScript = null; # Replace with Ojo-specific update script if applicable
    };

    meta = with lib; {
        homepage = "https://example.com/ojo-system-monitor";
        description = "Ojo System Monitor shows you system resource usage and running programs";
        mainProgram = "ojo-system-monitor";
        maintainers = [ "kunruh" ]; # Replace with actual maintainers
        license = licenses.gpl3; # Replace with the correct license
        platforms = platforms.linux;
    };
})
