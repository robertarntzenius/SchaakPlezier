import sys

from schaak_plezier.app import SchaakPlezier


def main():
    app = SchaakPlezier(sys.argv)

    app.build()

    app.run()


if __name__ == "__main__":
    main()
