GODOT_BIN ?= godot

.PHONY: run
run:
	$(GODOT_BIN) --headless --path $(CURDIR) --quit

