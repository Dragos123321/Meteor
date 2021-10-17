#pragma once

#include "piece.h"

class Queen : public Piece
{
	virtual void compute_available_movements(std::vector<Piece*>, std::vector<Piece*>) override;
	virtual std::string_view get_model_path() const override;
	virtual std::string_view get_name() const override;
};

