import sys

from schaak_plezier.gui import SchaakPlezier


def main():
    app = SchaakPlezier(sys.argv)

    app.build()

    app.run()


if __name__ == "__main__":
    main()
